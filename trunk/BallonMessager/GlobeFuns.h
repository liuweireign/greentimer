#pragma once

class GlobeFuns
{
public:
	// 取得程序运行的目录（以反斜线\结尾）
	static ATL::CString GetAppDirectory(bool bEndWithBackSlash=true);

	//////////////////////////////////////////////////////////////////////////
	//时间与字符串互相转换函数
	static ATL::CString TimeToString(CTime t);
	static ATL::CString TimeToFriendlyString(CTime t);
	static CTime StringToTime(ATL::CString strTime);
	//////////////////////////////////////////////////////////////////////////
};
