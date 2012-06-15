#include "stdafx.h"
#include "HttpServ.h"

#include "spporting.hpp"

#include "Globe.h"
#include "KeyValueDB.h"
#include "DBLog.h"
#include "DBSetting.h"
#include "GlobeFuns.h"


#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")

/*
void SP_HttpEchoHandler::handle( SP_HttpRequest * request, SP_HttpResponse * response ) {
	response->setStatusCode( 200 );

	string uri = request->getURI();

	if(uri=="/api/log"){  //写日志
		handlerLog(request,response);
		return;
	}
	else if(uri=="/api/readlog"){  //写日志
		handlerShowLog(request,response);
		return;
	}
	else if('/'==*uri.rbegin()){ //目录
		handlerDir(request,response);
		return;
	}
	else{						//文件
		handlerFile(request,response);
		return;
	}

	response->appendContent( "<html><head>"
		"<title>Welcome to simple http</title>"
		"</head><body><p>hello,world</p></body></html>" );
	return;
};


SP_HttpHandler * SP_HttpEchoHandlerFactory::create() const {
	return new SP_HttpEchoHandler();
};


HttpServ::HttpServ(void)
{
}

HttpServ::~HttpServ(void)
{
}

*/

void TServ::httpserv()
{
	/////////////初始化winsock环境
	WSADATA wsadata;
	WORD wVersion=MAKEWORD(2,0);/////////winsock 2.0
	if(WSAStartup(wVersion,&wsadata)!=0)
	{
		printf("initalize failed!/n");
		WSACleanup();
		exit(1);
	}

	int sock,csock,length;//sock是服务器端侦听套接字，csock是客户端连接套接字
	length=sizeof(struct sockaddr);


	struct sockaddr_in server_ipaddr,client_ipaddr;//用来获得服务器和客户端的地址信息
	memset(&server_ipaddr,0,length);
	server_ipaddr.sin_family=AF_INET;
	server_ipaddr.sin_port=htons(80);
	server_ipaddr.sin_addr.s_addr=inet_addr("0.0.0.0");



	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	///绑定
	if(bind(sock,(struct sockaddr *)&server_ipaddr,length)==SOCKET_ERROR)
	{
		printf("Bind error/n");
		WSACleanup();
		exit(1);
	}
	////侦听
	if(listen(sock,5)==SOCKET_ERROR)
	{
		printf("Listen error/n");
		WSACleanup();
		exit(1);
	}

	//////////
	while(1)
	{
		//////////侦听到连接后，产生新的套接字
		///////////用来和客户端传递消息
		csock=accept(sock,(struct sockaddr *)&client_ipaddr,&length);
		if(csock==SOCKET_ERROR)
		{
			printf("Listen error/n");
			WSACleanup();
			exit(1);
		}
		string strRecv;
		char buff[4096];
		int nbuff;
		while(true){
			nbuff=recv(csock,buff,4096,0);
			if (nbuff<=0)
			{
				break;
			}

			strRecv.append(buff,nbuff);
			if (strstr(strRecv.c_str(),"\r\n\r\n"))
			{
				break;
			}
		}
		TReq req(strRecv);
		TResp resp(csock);


		resp.setStatusCode( 200 );

		string uri = req.getURI();

		if(uri=="/api/log"){  //写日志
			handlerLog(&req,&resp);
			//return;
		}
		else if(uri=="/api/readlog"){  //写日志
			handlerShowLog(&req,&resp);
			//return;
		}
		else if('/'==*uri.rbegin()){ //目录
			handlerDir(&req,&resp);
			///return;
		}
		else{						//文件
			handlerFile(&req,&resp);
			//return;
		}

		resp.appendContent( "<html><head>"
			"<title>Welcome to simple http</title>"
			"</head><body><p>hello,world</p></body></html>" );
		resp.finish();
		//resp.appendContent("hello,world!\r\n");
		//resp.appendContent(req.getURI().c_str());
	}
}

TReq::TReq( const string &req )
{
	m_strReq = req;
}

std::string TReq::getURI()
{
	const char *pBgn = strstr(m_strReq.c_str(),"GET ");
	if(!pBgn){
		pBgn = strstr(m_strReq.c_str(),"get ");
	}
	if (!pBgn)
	{
		return "/";
	}
	pBgn += 4;
	const char *pEnd = strstr(pBgn," ");
	if (!pEnd)
	{
		return "/";
	}

	string strGet;
	strGet.append(pBgn,pEnd-pBgn);
	return strGet;
}

const char *TReq::getParamValue( const string &strName )
{
	string strURI = getURI();
	vector<string> strRes;
	StrFuns::Tokenize(strURI,strRes,"?&=");
	vector<string>::iterator it = strRes.begin();
	for (;it!=strRes.end();it++)
	{
		ATLTRACE("TOKEN:%s\n",(*it).c_str());
		if (strName==(*it))
		{
			it++;
			if (it==strRes.end())
			{
				return NULL;
			}
			return (*it).c_str();
		}
	}
	return NULL;
}

TResp::TResp( int socket )
{
	m_socket = socket;
	addHeader("Server","TS Server");
	addHeader("Content-Type","text/html");
	addHeader("Accept-Ranges","bytes");
	addHeader("Accept-Ranges","bytes");
	time_t lt = time(NULL);
	string strGmtNow = (string)ctime(&lt) + " GMT";
	addHeader("Date",strGmtNow.c_str());
}

void TResp::addHeader( const char *key,const char *value )
{
	m_mapHeader[key] = value;
}

void TResp::appendContent( const char *resp,int ilen /*= 0*/ )
{
	if (ilen<=0)
	{
		ilen = strlen(resp);
	}
	m_strResp.append(resp,ilen);
}

void TResp::finish()
{
	//////////要传送的网页内容
	//string CustomHtml = (string)"hello,world!\r\n" + req.getURI();
	//ATLTRACE("name:%s",req.getParamValue("name").c_str());

	//char headers[1000];//////////保存头部
	/////////头部格式
	char hdrFmt[]=
		"HTTP/1.0 200 OK\r\n"
		"\r\n";

	addHeader("Content-Length",GlobeFuns::Int2Str(m_strResp.size()).c_str());

	/////发送响应头部
	//send(m_socket,headers,strlen(headers),0);
	string strHead = "HTTP/1.0 200 OK\r\n";
	map<string,string>::iterator it = m_mapHeader.begin();
	for (;it!=m_mapHeader.end();it++)
	{
		strHead += (*it).first + ":" + (*it).second + "\r\n";
	}
	strHead += "\r\n";
	send(m_socket,strHead.data(),strHead.size(),0);


	/////发送内容 
	send(m_socket,m_strResp.data(),m_strResp.size(),0);

	/////////关闭本次连接
	closesocket(m_socket);
}