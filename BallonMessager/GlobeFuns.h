#pragma once

class GlobeFuns
{
public:
	static 	// 取得程序运行的目录（以反斜线\结尾）
		ATL::CString GetAppDirectory(bool bEndWithBackSlash=true);
};
