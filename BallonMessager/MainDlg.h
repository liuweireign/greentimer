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
#include "./DialogToDo.h"
#include "./DBLog.h"
#include "GlobeFuns.h"
#include "DialogToDoHistory.h"
#include "OpinionDlg.h"
#include "Globe.h"

//快捷键ID
const UINT uiACCELAR_ID_SHOWMAINDLG = 0X1000;
const UINT uiACCELAR_ID_OPENTASK = 0X1001;
const UINT uiACCELAR_ID_OPENNOTIFY = 0X1002;

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
		COMMAND_ID_HANDLER(ID_CMD_TODO, OnBtnTodo)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(ID_APP_OPINION, OnBtnOpinion)
		COMMAND_ID_HANDLER(ID_CMD_OPNION, OnBtnOpinion)
		COMMAND_ID_HANDLER(ID_CMD_TASKVIEW, OnTaskView)
		COMMAND_ID_HANDLER(ID_APP_TASKADD, OnBtnTaskAdd)
		COMMAND_ID_HANDLER(ID_APP_OPEN_TASKDB, OnBtnOpenTaskDB)
		COMMAND_ID_HANDLER(ID_APP_TODAYTASK, OnBtnTodayTask)
		COMMAND_ID_HANDLER(ID_CMD_QUICKREMIND, OnBtnQuickRemind)
		COMMAND_ID_HANDLER(ID_APP_TODOHISTORY, OnBtnTodoHistory)
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
		//g_DBLog.Log("MAIN",0,0,0,"启动");
		LOG_MAIN(LOG_CONST::MMV_START,0,"启动");
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
			CString strMsg = "合理安排时间，做个高效的人。\r\n";

			CTime tm = CTime::GetCurrentTime();
			ATL::CString strNow;
			strNow.Format("现在时间： %d 点 %d 分。\r\n\r\n",tm.GetHour(),tm.GetMinute());
			strMsg += strNow;

			int iFinished,iPlaned, iWorking;
			if(g_todoSet.GetStatic(iFinished,iPlaned, iWorking))
			{
				CString strTasks;
				//strTasks.Format("您有 %d 件计划任务，其中 %d 件正在处理中。",iPlaned+iWorking, iWorking);
				strTasks.Format("%d 个任务已完成，%d 个任务需要处理",iFinished,iPlaned+iWorking);
				strMsg += strTasks;
			}
			BalloonToolTips(strMsg);
		}

		//每隔一定时间检查一次，看有没有需要运行的任务
		SetTimer(0,10*1000,NULL);

		//显示当前是否已设置自启动
		char buf[_MAX_PATH];
		::GetModuleFileName(NULL,buf,_MAX_PATH);
		TSelfStart tss("GreenTimer",buf);
		CButton btn(GetDlgItem(IDC_CHK_SELFSTART));
		btn.SetCheck(tss.IsSelfStart());

		//注册快捷键
		DWORD dw = Globe::GetHotKeyOpenTask();
		ATLTRACE("dw1=%d,%d\n",LOWORD(dw),HIWORD(dw));
		//RegisterHotKey(m_hWnd,uiACCELAR_ID_SHOWMAINDLG,MOD_CONTROL,'G');
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENTASK,HIWORD(dw),LOWORD(dw));
		dw = Globe::GetHotKeyOpenNotify();
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENNOTIFY,HIWORD(dw),LOWORD(dw));

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		//反注册快捷键
		UnregisterHotKey(m_hWnd, uiACCELAR_ID_OPENTASK);
		UnregisterHotKey(m_hWnd, uiACCELAR_ID_OPENNOTIFY);

		//g_DBLog.Log("MAIN",0,1,0,"退出");
		LOG_MAIN(LOG_CONST::MMV_EXIT,0,"退出");

		return 0;
	}

	//快捷键处理
	LRESULT OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//id为你自己定义的一个ID值，
		//对一个线程来讲其值必需在0x0000 - 0xBFFF范围之内，
		//对DLL来讲其值必需在0xC000 - 0xFFFF 范围之内，在同一进程内该值必须唯一
		if (wParam==uiACCELAR_ID_OPENTASK)
		{
			OpenTodo();
			//this->PostMessage(WM_ICON,IDR_MAINFRAME,WM_RBUTTONUP);
		}
		else if (wParam == uiACCELAR_ID_OPENNOTIFY)
		{
			OpenTask();
		}

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
		COpinionDlg dlg;
		dlg.DoModal();

		//////////////////////////////////////////////////////////////////////////
		//重新注册快捷键
		//todo : 应当检查一下，并不需要每次都全部重新注册的。
		//////////////////////////////////////////////////////////////////////////

		//反注册快捷键
		UnregisterHotKey(m_hWnd, uiACCELAR_ID_OPENTASK);
		UnregisterHotKey(m_hWnd, uiACCELAR_ID_OPENNOTIFY);

		//注册快捷键
		DWORD dw = Globe::GetHotKeyOpenTask();
		ATLTRACE("dw1=%d,%d\n",LOWORD(dw),HIWORD(dw));
		//RegisterHotKey(m_hWnd,uiACCELAR_ID_SHOWMAINDLG,MOD_CONTROL,'G');
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENTASK,HIWORD(dw),LOWORD(dw));
		dw = Globe::GetHotKeyOpenNotify();
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENNOTIFY,HIWORD(dw),LOWORD(dw));
		return 0;
	}

	LRESULT OnBtnTaskAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

		//CTaskListDialog dlg;
		//dlg.DoModal();

		OpenTask();
		return 0;
	}

	LRESULT OnBtnOpenTaskDB(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(NULL,NULL,GlobeFuns::GetAppDirectory().c_str(),NULL,NULL,SW_SHOW);
		
		return 0;
	}

	LRESULT OnBtnTodayTask(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAddTodayTaskDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnBtnTodoHistory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CDialogToDoHistory dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnBtnTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		OpenTodo();
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

		LOG_NOTIFY(LOG_CONST::MNV_DISPLAY,task.Id,"显示提示");
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
	//用此函数todo，避免多次启动todo实例。使用快捷键的时候很容易导致这种问题。
	void OpenTask()
	{
		DoModalOnce<CTaskListDialog>();
	}
	//用此函数todo，避免多次启动todo实例。使用快捷键的时候很容易导致这种问题。
	void OpenTodo()
	{
		DoModalOnce<DialogToDo>();
	}
private:

	//模板函数：T_dlg类型的对话框同时只能domodal一个
	//再次调用的时候，如果发现该对话框已经存在，则闪烁窗口提示
	template<class T_dlg>
	void DoModalOnce()
	{
		static T_dlg *pDlg  = NULL;
		if (pDlg==NULL)
		{
			T_dlg dlg;
			pDlg = &dlg;
			dlg.DoModal();
			pDlg = NULL;
		}
		else
		{
			pDlg->ShowWindow(SW_SHOWNORMAL);
			pDlg->BringWindowToTop();
			pDlg->FlashWindow(TRUE);
		}
	}
};
