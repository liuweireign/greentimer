#pragma once
#include <vector>
#include <map>
#include <string>

#include "StrFuns.h"

#include "winsock.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#pragma comment (lib,"wsock32")///所需的库文件

class TReq
{
public:
	TReq(const string &req);
	string getURI();
	const char *getParamValue(const string &strName);
private:
	string m_strReq;
};

class TResp
{
public:
	TResp(int socket);
	void setStatusCode(int code){};
	void addHeader(const char *key,const char *value);
	void appendContent(const char *resp,int ilen = 0);

	void finish();
private:
	int m_socket;
	string m_strResp;
	map<string,string> m_mapHeader;
};


class TServ
{
public:
	static void httpserv();
};

