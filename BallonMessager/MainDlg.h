// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShellIcon.h"
#include "MyIconMenu.h"
#include "TaskDB.h"
#include "AddTodayTaskDlg.h"
#include "TaskListDialog.h"
#include "TaskModifyDialog.h"
#include "QuickRemind.h"
#include "TSelfStart.h"

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
		COMMAND_ID_HANDLER(ID_CMD_QUICKREMIND, OnBtnQuickRemind)
		COMMAND_ID_HANDLER(ID_MENU_QUICKREMIND_5MIN, OnBtnQuickRemind)
		COMMAND_ID_HANDLER(ID_MENU_QUICKREMIND_10MIN, OnBtnQuickRemind)
		COMMAND_ID_HANDLER(ID_MENU_QUICKREMIND_1HOUR, OnBtnQuickRemind)
		COMMAND_ID_HANDLER(ID_MENU_QUICKREMIND_TOMORROW, OnBtnQuickRemind)
		COMMAND_HANDLER(IDC_CHK_SELFSTART, BN_CLICKED, OnBnClickedChkSelfstart)
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
		g_TaskDB.ReadFromDB();

		//如果当前没有任务，显示一下欢迎信息
		ITask task;
		if (!g_TaskDB.FindTaskRunNow(task))
		{
			CTime tm = CTime::GetCurrentTime();
			WTL::CString strNow;
			strNow.Format("合理安排时间，做个高效的人。\r\n\r\n现在时间： %d 点 %d 分。",tm.GetHour(),tm.GetMinute());
			BalloonToolTips(strNow);
		}

		//每隔一定时间检查一次，看有没有需要运行的任务
		SetTimer(0,10*1000,NULL);

		char buf[_MAX_PATH];
		::GetModuleFileName(NULL,buf,_MAX_PATH);
		TSelfStart tss("GreenTimer",buf);

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

	LRESULT OnBtnQuickRemind(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int iTime = 10;
		switch(wID)
		{
		case ID_MENU_QUICKREMIND_5MIN:
			iTime = 5;
			break;
		case ID_MENU_QUICKREMIND_10MIN:
			iTime = 10;
			break;
		case ID_MENU_QUICKREMIND_1HOUR:
			iTime = 60;
		    break;
		case ID_MENU_QUICKREMIND_TOMORROW:
			iTime = 24*60;
		    break;
		default:
			iTime = 10;
		    break;
		}
		CQuickRemind dlg(iTime);
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

	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//查询：有没有任务需要运行了？
		ITask task;
		if (!g_TaskDB.FindTaskRunNow(task))
		{
			return 0;
		}

		//提示
		BalloonToolTips(task.Tip);

		//记录本次运行时间
		task.LastRunTime = CTime::GetCurrentTime();
		g_TaskDB.UpdateTask(task);
		g_TaskDB.SaveToDB();

		return 0;
	}

	//自启动选项点击事件
	LRESULT OnBnClickedChkSelfstart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		char buf[_MAX_PATH];
		::GetModuleFileName(NULL,buf,_MAX_PATH);
		TSelfStart tss("GreenTimer",buf);

		CButton btn(GetDlgItem(IDC_CHK_SELFSTART));
		if(btn.GetCheck())
		{
			tss.SetSelfStart();
		}
		else
		{
			tss.RemoveSelfStart();
		}

		btn.SetCheck(tss.IsSelfStart());

		return 0;
	}

};
