#include "stdafx.h"
#include ".\globefuns.h"

// 取得程序运行的目录（以反斜线\结尾）
ATL::CString GlobeFuns::GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
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