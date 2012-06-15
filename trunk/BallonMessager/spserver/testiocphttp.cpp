/*
 * Copyright 2008 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <string>
using namespace std;

#include "spporting.hpp"

#include "spwin32iocp.hpp"
#include "spiocpserver.hpp"

#include "sphttp.hpp"
#include "sphttpmsg.hpp"
#include "spserver.hpp"

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")

class SP_HttpEchoHandler : public SP_HttpHandler {
public:
	SP_HttpEchoHandler(){}
	virtual ~SP_HttpEchoHandler(){}

	virtual void handle( SP_HttpRequest * request, SP_HttpResponse * response ) {

		string url = request->getURI(); 
		if(std::string("/read")==url){
			response->setStatusCode( 200 );
			response->appendContent( "<html><head>"
				"<title>Welcome to simple http</title>"
				"</head><body>" );
			response->appendContent( "hello,reader!" );
			response->appendContent( "</body></html>\n" );
			return;
		}

		response->setStatusCode( 200 );


		response->appendContent( "<html><head>"
			"<title>Welcome to simple http</title>"
			"</head><body>" );

		char buffer[ 512 ] = { 0 };
		snprintf( buffer, sizeof( buffer ),
			"<p>The requested URI is : %s.</p>", request->getURI() );
		response->appendContent( buffer );

		snprintf( buffer, sizeof( buffer ),
			"<p>Client IP is : %s.</p>", request->getClientIP() );
		response->appendContent( buffer );

		int i = 0;

		for( i = 0; i < request->getParamCount(); i++ ) {
			snprintf( buffer, sizeof( buffer ),
				"<p>Param - %s = %s<p>", request->getParamName( i ), request->getParamValue( i ) );
			response->appendContent( buffer );
		}

		for( i = 0; i < request->getHeaderCount(); i++ ) {
			snprintf( buffer, sizeof( buffer ),
				"<p>Header - %s: %s<p>", request->getHeaderName( i ), request->getHeaderValue( i ) );
			response->appendContent( buffer );
		}

		if( NULL != request->getContent() ) {
			response->appendContent( "<p>" );
			response->appendContent( request->getContent(), request->getContentLength() );
			response->appendContent( "</p>" );
		}

		response->appendContent( "</body></html>\n" );
	}
};

class SP_HttpEchoHandlerFactory : public SP_HttpHandlerFactory {
public:
	SP_HttpEchoHandlerFactory(){}
	virtual ~SP_HttpEchoHandlerFactory(){}

	virtual SP_HttpHandler * create() const {
		return new SP_HttpEchoHandler();
	}
};

void StartAndWaitForClient(){
	char szFilePath[MAX_PATH + 256]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, ('\\')))[1] = 0;//删除文件名，只获得路径字串
	string strExePath = szFilePath;
	strExePath += "main.exe";


	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL;

	PROCESS_INFORMATION pi; 
	STARTUPINFO si;

	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	memset(&si, 0, sizeof(STARTUPINFO));

	//si.cb = sizeof(STARTUPINFO); 
	//si.dwFlags = STARTF_USESTDHANDLES;
	//si.hStdError = pcs->hErrorU;
	//si.hStdInput = pcs->hWriteU;
	//si.hStdOutput = pcs->hReadU;

	char pathbuf[10240]="";
	if(!CreateProcess(strExePath.c_str(), pathbuf, &saAttr, &saAttr, TRUE, CREATE_NO_WINDOW, 0, 0, &si, &pi))
	{
		//DestroyProcessWithPipe(pcs);
		return;
	}

	Globe::hProcess = pi.hProcess;
	WaitForInputIdle(pi.hProcess, 10000);  
	//等待该进程结束 
	WaitForSingleObject(pi.hProcess,   INFINITE); 
}
int main( int argc, char * argv[] )
{
	int port = 18280, maxThreads = 10;
	const char * serverType = "hahs";

	if( 0 != sp_initsock() ) assert( 0 );

	SP_IocpServer server( "", port, new SP_HttpHandlerAdapterFactory( new SP_HttpEchoHandlerFactory() ) );

	server.setTimeout( 60 );
	server.setMaxThreads( maxThreads );
	server.setReqQueueSize( 100, "HTTP/1.1 500 Sorry, server is busy now!\r\n" );

	//server.runForever();
	server.run();

	StartAndWaitForClient();

	sp_closelog();

	return 0;
}

