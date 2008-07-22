// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShellIcon.h"
#include "MyIconMenu.h"
//#include "AboutDlg.h"
#include "OpinionDlg.h"
#include "TaskDB.h"
#include "TaskViewDlg.h"
#include "SimpleTaskViewDlg.h"
#include "AddTodayTaskDlg.h"
#include "TaskListDialog.h"
#include "TaskModifyDialog.h"

#include <atlctrls.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atltime.h>
#include <string>

class CMainDlg : 
	public CDialogImpl<CMainDlg>, 
	public CUpdateUI<CMainDlg>,
	public CMessageFilter, 
	public CIdleHandler,
	public CShellIcon<CMainDlg, CMyIconMenu<CMainDlg>, IDR_MENU_ICON>
{
public:
	typedef CShellIcon<CMainDlg, CMyIconMenu<CMainDlg>, IDR_MENU_ICON> CMyShellIcon;

	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(ID_APP_OPINION, OnBtnOpinion)
		COMMAND_ID_HANDLER(ID_CMD_TASKVIEW, OnTaskView)
		COMMAND_ID_HANDLER(ID_APP_TODAYTASK, OnBtnTodayTask)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		CHAIN_MSG_MAP(CMyShellIcon)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		CreateShellIcon();

		_iLastRemindMin = -1;
		_bFirstTimeUp = true;

		g_TaskDB.ReadFromDB();

		SetTimer(0,3*1000,NULL);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnBtnOpinion(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//COpinionDlg dlg;
		//dlg.DoModal();
		CTaskModifyDialog dlg(0);
		dlg.DoModal();
		return 0;
	}

	LRESULT OnTaskView(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//TaskViewDlg dlg;
		//CSimpleTaskViewDlg dlg;
		CTaskListDialog dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnBtnTodayTask(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAddTodayTaskDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		//CloseDialog(wID);
		ShowWindow(SW_HIDE);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//CloseDialog(wID);
		ShowWindow(SW_HIDE);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
public:
	//LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	int _iLastRemindMin;
	bool _bFirstTimeUp;


	// 取得程序运行的目录（以反斜线\结尾）
	ATL::CString GetAppDirectory(bool bEndWithBackSlash=true)
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

	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//////////////////////////////////////////////////////////////////////////
		ITask task;
		if (g_TaskDB.FindTaskRunNow(task))
		{
			//提示
			BalloonToolTips(task.Tip);

			//记录本次运行时间
			task.LastRunTime = CTime::GetCurrentTime();
			g_TaskDB.UpdateTask(task);
			g_TaskDB.SaveToDB();
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////
		//GetPrivateProfileString(strSection,strKey,"COM3",buffer,sizeof(buffer),strFileName);  
		//整点报时功能。
		//CTime tm = CTime::GetCurrentTime();
		//int iNowHour = tm.GetHour();
		//int iNowMin = tm.GetMinute();

		//ATL::CString strFileName = GetAppDirectory()+"tips.ini";
		//char *strDefaultTips = "合理安排时间，做个高效的人。";
		//char *strEmptyTips = "";
		//char buffer[2*1024];
		//char hour[8]={0};
		//itoa(iNowHour,hour,10);
		//char min[8]={0};
		//itoa(iNowMin,min,10);

		//ATL::CString key = hour;
		//key += "-";
		//key += min;

		//int iCharsRt = GetPrivateProfileString("tips",key,strEmptyTips,buffer,sizeof(buffer),strFileName);

		////有没有为现在这个时间设定提醒呢？现在这个提示显示过没有？
		//if (iCharsRt != 0 && iNowMin != _iLastRemindMin)
		//{
		//	ATL::CString strNow;

		//	//modify by tianzuo,2008-5-14. 
		//	//bug NO.1 found by jameyu,2008-5-14
		//	//strNow.Format("现在时间： %d 点整。\r\n\r\n%s",iNowHour,buffer);
		//	strNow.Format("现在时间： %d 点 %d 分。\r\n\r\n%s",iNowHour,tm.GetMinute() ,buffer);
		//	//end modify by tianzuo,2008-5-14. 

		//	BalloonToolTips(strNow);
		//	_iLastRemindMin = iNowMin;
		//	_bFirstTimeUp = false;
		//}
		////没有为当前时间设定提醒，显示一下欢迎信息
		//else
		//{
		//	if (_bFirstTimeUp)
		//	{
		//		ATL::CString strMsg;
		//		strMsg.Format("现在时间： %d 点 %d 分。\r\n\r\n%s",iNowHour,tm.GetMinute() ,strDefaultTips);
		//		BalloonToolTips(strMsg);
		//		_bFirstTimeUp = false;
		//	}
		//}

		return 0;
	}

};
