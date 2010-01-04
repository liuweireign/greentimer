#include ".\uploadhandler.h"
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


// 取得程序运行的目录（以反斜线\结尾）
static std::string GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
{
	//取系统路径
	char buf[_MAX_PATH];
	::GetModuleFileName(NULL,buf,_MAX_PATH);
	int iIndex = (int)std::string(buf).rfind('\\');
	if(bEndWithBackSlash)
	{
		buf[iIndex+1]='\0';	//截断，获得路径名(保留最后的'\')
	}
	else
	{
		buf[iIndex]='\0';	//截断，获得路径名(去掉最后的'\')
	}
	return (char *)buf;
}


UploadHandler::UploadHandler()
{
}

UploadHandler::~UploadHandler(void)
{
}

void UploadHandler::handle( SP_HttpRequest * request, SP_HttpResponse * response )
{
	string url = request->getURL();
	if(url=="/")
	{
		url = "/index.htm";
	}
	std::replace(url.begin(),url.end(),'/','\\');
	
	//const char *full_path = request->getParamValue("path");
	string full_path = GetAppDirectory(false) + url;
	
	//////////////////////////////////////////////////////////////////////////
	//打开文件
	//////////////////////////////////////////////////////////////////////////
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is)
	{
		ReturnNotFound(response);
		return;
	}

	is.seekg(0);
	if(is.eof() || is.fail())  //fuck,seekg居然连个返回false都不做
	{
		ReturnNotFound(response);
		return;
	}

	//发送数据体
	//unsigned int iLeftSize = iSize;
	while (true)
	{
		int iReadSize = is.read(m_pBuffer, c_iBUFFER_SIZE).gcount();
		//iLeftSize -= iReadSize;
		if(iReadSize <= 0)
		{
			break;
		}
		response->appendContent(m_pBuffer, iReadSize);
	}
}

void UploadHandler::ReturnNotFound( SP_HttpResponse * response)
{
	const char *strEmptyMsg = "Empty!";
	//构造数据包
	response->appendContent(strEmptyMsg,strlen(strEmptyMsg));
}