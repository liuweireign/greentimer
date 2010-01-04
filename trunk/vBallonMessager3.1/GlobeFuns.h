#pragma once

class GlobeFuns
{
public:
	// 取得程序运行的目录（以反斜线\结尾）
	static std::string GetAppDirectory(bool bEndWithBackSlash=true);

	//////////////////////////////////////////////////////////////////////////
	//时间与字符串互相转换函数
	static ATL::CString TimeToString(const CTime &t);
	static ATL::CString TimeToFriendlyString(const CTime &t);
	static CTime StringToTime(const ATL::CString &strTime);
	//////////////////////////////////////////////////////////////////////////

	static UINT TimeToInt(const CTime &t);
	static CTime IntToTime(UINT uiTime);
};
