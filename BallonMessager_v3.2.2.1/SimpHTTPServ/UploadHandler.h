#pragma once
#ifndef WIN32
#define WIN32
#endif

#include "./spserver/spporting.hpp"
#include "./spserver/spwin32iocp.hpp"
#include "./spserver/spiocpserver.hpp"
#include "./spserver/sphttp.hpp"
#include "./spserver/sphttpmsg.hpp"
#include "./spserver/spserver.hpp"

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")

//处理每个连接的缓冲区大小
//即每次读磁盘的大小。设置值太小可能导致读磁盘次数过多。太大则浪费内存。
const int c_iBUFFER_SIZE = 8*1024;

class UploadHandler :
	public SP_HttpHandler
{
public:
	UploadHandler();
	~UploadHandler(void);

	virtual void handle( SP_HttpRequest * request, SP_HttpResponse * response );
private:
	char m_pBuffer[c_iBUFFER_SIZE];
private:
	void ReturnNotFound(SP_HttpResponse * response);
};
