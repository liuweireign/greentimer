// stdafx.cpp : source file that includes just the standard includes
//	BallonMessager.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

#include <ATLComTime.h>

using namespace ATL;
//////////////////////////////////////////////////////////////////////////
//时间与字符串互相转换函数
ATL::CString TimeToString(CTime t)
{
	ATL::CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

ATL::CTime StringToTime(ATL::CString strTime)
{
	COleDateTime   tm;  
	tm.ParseDateTime(strTime);  
	SYSTEMTIME   st;  
	tm.GetAsSystemTime(st);  
	return CTime(st);   
}
//////////////////////////////////////////////////////////////////////////
