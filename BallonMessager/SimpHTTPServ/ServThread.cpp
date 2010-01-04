#include ".\servthread.h"
#include "UploadHandler.h"
#include <assert.h>


class SP_HttpEchoHandlerFactory : public SP_HttpHandlerFactory {
public:
	SP_HttpEchoHandlerFactory(){
	}
	virtual ~SP_HttpEchoHandlerFactory(){}

	virtual SP_HttpHandler * create() const {
		return new UploadHandler();
	}
private:
};

ServThread::ServThread()
{
	m_pSpServer = NULL;
}

ServThread::~ServThread(void)
{
}

void * ServThread::Thread()
{
	ThreadStarted();

	int port = 8080, maxThreads = 10;
	const char * serverType = "hahs";

	if( 0 != sp_initsock() ) assert( 0 );

	m_pSpServer = new SP_IocpServer( "", port, new SP_HttpHandlerAdapterFactory( new SP_HttpEchoHandlerFactory() ) );

	m_pSpServer->setTimeout( 60 );
	m_pSpServer->setMaxThreads( maxThreads );
	m_pSpServer->setReqQueueSize( 100, "HTTP/1.1 500 Sorry, server is busy now!\r\n" );
	m_pSpServer->runForever();

	sp_closelog();
	return NULL;
}

void ServThread::shutdown()
{
	m_pSpServer->shutdown();
}