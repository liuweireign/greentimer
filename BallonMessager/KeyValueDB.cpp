#include "stdafx.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <algorithm>

#include "KeyValueDB.h"
#include "DBSetting.h"
#include "strCoding.h"
#include "Mime.h"
#include "GlobeFuns.h"
#include "DBLog.h"
#include "Globe.h"

const char *SafeGetParam(TReq * request,const char *name,const char *emptyRes=""){
	const char *pData = request->getParamValue(name);
	if (pData)
	{
		return pData;
	}
	return emptyRes;
}

int SafeGetParamInt(TReq * request,const char *name,int emptyRes=0){
	const char *pData = request->getParamValue(name);
	if (pData)
	{
		return atoi(pData);
	}
	return emptyRes;
}
//
//void testHandler(TReq * request, TResp * response){
//	const char *url = SafeGetParam(request,"url");
//	if(strlen(url)<=0){
//		response->appendContent("add the param 'url', please!");
//		return;
//	}
//	std::string strRes = httpCpp::httpget(url);
//	//std::cout<<strRes<<std::endl;
//	response->appendContent(strRes.c_str());
//	return;
//}

void handlerMain(TReq * request, TResp * response){
	response->appendContent("<html><title>aaa</title><body><font color=red>C++ Flash loader</font></body></html>");
	return;
}

string GetCurrProcessUser()
{	
	BOOL bRet(TRUE);
	string strName;

	DWORD dwSize = MAX_PATH;
	TCHAR *pszName = new TCHAR[dwSize];
	if (!GetUserName(pszName, &dwSize))
	{
		delete[] pszName;
		pszName = new TCHAR[dwSize];
		bRet = GetUserName(pszName, &dwSize);
	}

	strName = pszName;
	delete[] pszName;		
	return strName;
	//return bRet;
}  

//========================================
/*
//写键值
void handlerWrite(TReq * request, TResp * response){
	const char *key = SafeGetParam(request,"key");
	if(strlen(key)<=0){
		response->appendContent("{\"res\":-1,\"msg\":\"key not found\"}");
		return;
	}
	const char *value = SafeGetParam(request,"value");
	if(strlen(value)<=0){
		response->appendContent("{\"res\":-1,\"msg\":\"value not found\"}");
		return;
	}

	strCoding code;
	string text = code.UrlUTF8Decode(value);

	DBSetting::dbSetting.SetValue(key,text);
	string strRes = (string)"{\"res\":0,\"msg\":\"success\",\"key\":\"" + key + "\",\"value\":\"" + text + "\"}";
	response->appendContent(strRes.c_str());
	return;
}
//读键值
void handlerRead(TReq * request, TResp * response){

	const char *key = SafeGetParam(request,"key");
	if(strlen(key)<=0){
		response->appendContent("{\"res\":-1,\"msg\":\"key not found\"}");
		return;
	}
	//::EnumWindows()
	string value("NULL");
	DBSetting::dbSetting.GetValue(key,value);
	string strRes = (string)"{\"res\":0,\"msg\":\"success\",\"key\":\"" + key + "\",\"value\":\"" + value + "\"}";
	response->appendContent(strRes.c_str());
	return;
}
*/
//写日志：模块，重要度，文本
void handlerLog(TReq * request, TResp * response){
	const char *modal = SafeGetParam(request,"modal","UNDEFINED");
	const int level = SafeGetParamInt(request,"level",0);

	strCoding code;
	string text = code.UrlUTF8Decode(SafeGetParam(request,"text"));
	int type = 1;
	int param2 = 0;
	g_DBLog.Log(modal,type,level,param2,text.c_str());
	string strRes = (string)"{\"res\":0,\"msg\":\"success\",\"modal\":\"" + modal + "\"}";
	response->appendContent(strRes.c_str());
	return;
}

//写日志：模块，重要度，文本
void handlerShowLog(TReq * request, TResp * response){
	const int offset = SafeGetParamInt(request,"offset",0);
	const int count = SafeGetParamInt(request,"count",100);
	string strLog = g_DBLog.ReadLog(offset,count);
	response->appendContent(strLog.c_str());
	return;
}

const string GetExePath(){
	char szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, ('\\')))[1] = 0;//删除文件名，只获得路径字串
	return szFilePath;
}

const string strServDir = GetExePath() + "website";

void RespMsg(TResp * response,const string &msg,const string &head=""){
	std::string strResp = (string)"<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=gbk\" /> <title>并行编译大师</title>" 
		+ head 
		+ "</head><body>" 
		+ msg 
		+ "</body></html>";
	response->addHeader("Content-Type","text/html; charset=GBK");
	response->appendContent((char *)strResp.c_str(),(int)strResp.size());
}
void RespJson(TResp * response,const string &msg){
	//response->addHeader("Content-Type","text/html; charset=GBK");
	response->appendContent((char *)msg.c_str(),(int)msg.size());
}


//FILE *fp;
void findFile(const char filePath[], string &strRes )
{
	//char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	//char szFile[MAX_PATH];

	strRes = "<ul>";

	//strncpy_s(szFind,filePath,MAX_PATH);
	//strncat(szFind,"\\*.*",MAX_PATH);//利用通配符找这个目录下的所以文件，包括目录
	string szFind = (string)filePath + "\\*.*";

	hFind=FindFirstFile(szFind.c_str(),&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return;

	while(TRUE)
	{

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//这是目录
		{
			if(string(FindFileData.cFileName)!="."){
				strRes += (string)"<li><a href='" + FindFileData.cFileName + "/'>[目录]" + FindFileData.cFileName + "</a></li>";
			}

		}
		else
		{
			strRes += (string)"<li><a href='" + FindFileData.cFileName + "'>" + FindFileData.cFileName + "</a></li>";
		}

		if(!FindNextFile(hFind,&FindFileData))//寻找下一个文件
			break;
	}
	strRes += "</ul>";
	FindClose(hFind);//关闭句柄
}
//文件服务器
void handlerDir(TReq * request, TResp * response){
	string strRes;
	string strDir = strServDir + request->getURI();
	findFile(strDir.c_str(),strRes);
	RespMsg(response,strRes);
}

//文件服务器
void handlerFile(TReq * request, TResp * response){

	string url = request->getURI();
	//replace(url.begin(),url)
	//url.
	string full_path = strServDir + url;

	string ext = url.substr(url.rfind(".")+1);
	transform(ext.begin(),ext.end(),ext.begin(),tolower);
	string strMime = Mime::ext2mime(ext);
	if(!strMime.empty()){
		response->addHeader("Content-Type",strMime.c_str());
	}

	//const char *fid = request->getParamValue("fid");
	const char *pos = request->getParamValue("pos");
	const char *size = request->getParamValue("size");
	int iPos = 0;
	unsigned int iSize = -1;
	if(pos){
		iPos = atoi(pos);
		iSize = atoi(size);
		if (iPos<0 || iSize<=0)
		{
			RespMsg(response,"文件位置或大小参数错误！");
			return;
		}
	}

	FILE *file = fopen(full_path.c_str(),"rb");
	if (!file)
	{
		DWORD error = GetLastError();
		//RespMsg(response,((string)"file not found!") + GlobeFuns::Int2Str(error));
		//RespMsg(response,((string)"file not found!") + GlobeFuns::Int2Str(error));
		response->setStatusCode(404);
		response->appendContent("0",1);
		//fclose(file); //打开不成功的，不能关闭
		return;
	}
	//HFILE hFile = OpenFile(full_path.c_str(),OF_READ);
	fseek(file,iPos,SEEK_SET);
	//发送数据体，注意，仅发送指定的部分
	unsigned int iLeftSize = iSize;
	while (iLeftSize>0)
	{
		const int c_iBUFFER_SIZE = 1024*64;
		char m_pBuffer[c_iBUFFER_SIZE];
		int iReadSize = (int)fread(m_pBuffer,sizeof(char), min(iLeftSize,c_iBUFFER_SIZE), file);
		iLeftSize -= iReadSize;
		if(iReadSize <= 0)
		{
			break;
		}
		response->appendContent(m_pBuffer, iReadSize);
	}
	fclose(file);
	return;
}

//
//void KeyValueDB::Register( HttpUploader *pHandler )
//{
//	pHandler->AddUrlHandler("/",handlerMain);
//
//	pHandler->AddUrlHandler("/api/system",handlerReadSys);
//	pHandler->AddUrlHandler("/api/write",handlerWrite);
//	pHandler->AddUrlHandler("/api/read",handlerRead);
//	pHandler->AddUrlHandler("/gnugo",handlerGnugo);
//}