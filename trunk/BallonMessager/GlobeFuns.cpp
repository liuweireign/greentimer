#include "stdafx.h"
#include ".\globefuns.h"

#include <ATLComTime.h>

using namespace ATL;

// 取得程序运行的目录（以反斜线\结尾）
std::string GlobeFuns::GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
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

//////////////////////////////////////////////////////////////////////////
//时间与字符串互相转换函数
ATL::CString GlobeFuns::TimeToString(const CTime &t)
{
	ATL::CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

ATL::CString GlobeFuns::TimeToFriendlyString(const CTime &t)
{
	CTimeSpan tmSpan = CTime::GetCurrentTime() - t;
	CString strRet;
	if (tmSpan.GetTotalMinutes()<1)
	{
		return "现在";
	}
	//if (tmSpan.GetTotalMinutes()<10)
	//{
	//	strRet.Format("%d分钟前",tmSpan.GetTotalMinutes());
	//	return strRet;
	//}
	if (tmSpan.GetTotalMinutes()<60)
	{
		strRet.Format("%d分钟前",tmSpan.GetTotalMinutes());
		return strRet;
	}
	if (tmSpan.GetTotalHours()<24)
	{
		strRet.Format("%d小时前",tmSpan.GetTotalHours());
		return strRet;
	}

	strRet.Format("%d天前",tmSpan.GetTotalHours()/24);
	return strRet;
}
ATL::CTime GlobeFuns::StringToTime(const ATL::CString &strTime)
{
	COleDateTime   tm;  
	tm.ParseDateTime(strTime);  
	SYSTEMTIME   st;  
	tm.GetAsSystemTime(st);  
	return CTime(st);   
}

UINT GlobeFuns::TimeToInt( const CTime &t )
{
	CTimeSpan ts = t-CTime(0);
	return (UINT)ts.GetTotalSeconds();
}

CTime GlobeFuns::IntToTime(UINT uiTime)
{
	return CTime(0)+CTimeSpan(0,0,0,uiTime);
}
//////////////////////////////////////////////////////////////////////////
