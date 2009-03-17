#pragma once
#include <string>
#include <vector>
#include <deque>

class NetLogger
{
public:
	//写日志
	//此函数是线程安全的，可以同时调用。
	static void WriteLog(unsigned int uiUserID,unsigned int uiLogCode,const char *logData);
};
