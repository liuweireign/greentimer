#include "StdAfx.h"
#include "MainDlg.h"
#include <atltime.h>
#include <string>

// 取得程序运行的目录（以反斜线\结尾）
WTL::CString GetAppDirectory(bool bEndWithBackSlash=true)
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

LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//GetPrivateProfileString(strSection,strKey,"COM3",buffer,sizeof(buffer),strFileName);  
	//整点报时功能。
	CTime tm = CTime::GetCurrentTime();
	int iNowHour = tm.GetHour();
	if (iNowHour!=_iLastRemindHour)
	{
		WTL::CString strFileName = GetAppDirectory()+"tips.ini";
		char *strDefaultTips = "合理安排时间，做个高效的人。";
		char buffer[2*1024];
		char key[16];
		itoa(iNowHour,key,10);
		GetPrivateProfileString("tips",key,strDefaultTips,buffer,sizeof(buffer),strFileName);

		WTL::CString strNow;
			strNow.Format("现在时间：%d点。\r\n\r\n",iNowHour);
		BalloonToolTips(strNow + (char *)buffer);
		_iLastRemindHour = iNowHour;
	}
	
	return 0;
}
