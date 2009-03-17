#include "StdAfx.h"
#include "NetLogger.h"
#include "../curl/curl.h"
#include "./URLEncode.h"

//真正负责写日志的函数
DWORD WINAPI ThreadWriteLog(LPVOID s)
{
	char *logString = (char *)s;
	std::string strUrl = std::string("http://routebest.appspot.com/") 
		+ logString;
	delete[] logString;

	CURL *curl = curl_easy_init();
	if(!curl) {
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	CURLcode res = curl_easy_perform(curl);

	if(0!=res)
	{
		return 0;
	}

	/* always cleanup */
	curl_easy_cleanup(curl);

	return 0;
}

//写日志
//此函数是线程安全的，可以同时调用。
void NetLogger::WriteLog(unsigned int uiUserID,unsigned int uiLogCode,const char *logData)
{
	//格式：http://loghost/log?userid=***&verify=***&code=***&data=*** 
	int verifycode = 0;
	char *logString = new char[4096];
	sprintf_s(logString,4096,"log?userid=%d&verify=%d&code=%d&data=%s",
		uiUserID,verifycode,uiLogCode,URLEncode::escapeURL(logData).c_str());
	CreateThread(NULL, 0, ThreadWriteLog, logString, 0, NULL);
}