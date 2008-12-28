// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TaskListDialog.h"
#include "DialogToDo.h"


#include "ShellIcon.h"
#include "MyIconMenu.h"
#include "TaskDB.h"
#include "./DBLog.h"
#include "GlobeFuns.h"
#include "TSelfStart.h"
#include "Globe.h"

#define WINDOW_MENU_POSITION	3

//快捷键ID
const UINT uiACCELAR_ID_SHOWMAINDLG = 0X1000;
const UINT uiACCELAR_ID_OPENTASK = 0X1001;
const UINT uiACCELAR_ID_OPENNOTIFY = 0X1002;


class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler,
	public CShellIcon<CMainFrame, CMyIconMenu<CMainFrame>, IDR_MENU_ICON>
{
public:
	typedef CShellIcon<CMainFrame, CMyIconMenu<CMainFrame>, IDR_MENU_ICON> CMyShellIcon;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CTabView m_view;
	CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE, OnWindowClose)
		COMMAND_ID_HANDLER(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)

		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CMyShellIcon)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		CMenuHandle menuMain = m_CmdBar.GetMenu();
		m_view.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));

		LOG_MAIN(LOG_CONST::MMV_START,0,"启动");

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
				strTasks.Format("您有 %d 件计划任务，其中 %d 件正在处理中。",iPlaned+iWorking, iWorking);
				strMsg += strTasks;
			}
			BalloonToolTips(strMsg);
		}

		//每隔一定时间检查一次，看有没有需要运行的任务
		SetTimer(0,10*1000,NULL);

		//显示当前是否已设置自启动
		//char buf[_MAX_PATH];
		//::GetModuleFileName(NULL,buf,_MAX_PATH);
		//TSelfStart tss("GreenTimer",buf);
		//CButton btn(GetDlgItem(IDC_CHK_SELFSTART));
		//btn.SetCheck(tss.IsSelfStart());

		//注册快捷键
		DWORD dw = Globe::GetHotKeyOpenTask();
		ATLTRACE("dw1=%d,%d\n",LOWORD(dw),HIWORD(dw));
		//RegisterHotKey(m_hWnd,uiACCELAR_ID_SHOWMAINDLG,MOD_CONTROL,'G');
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENTASK,HIWORD(dw),LOWORD(dw));
		dw = Globe::GetHotKeyOpenNotify();
		RegisterHotKey(m_hWnd,uiACCELAR_ID_OPENNOTIFY,HIWORD(dw),LOWORD(dw));

		CTaskListDialog* pView = new CTaskListDialog;
		pView->Create(m_view);
		m_view.AddPage(pView->m_hWnd, _T("定时提醒"));

		DialogToDo *pTodo = new DialogToDo;
		pTodo->Create(m_view);
		m_view.AddPage(pTodo->m_hWnd,_T("任务管理"));

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//CGreenKnowView* pView = new CGreenKnowView;

		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int nActivePage = m_view.GetActivePage();
		if(nActivePage != -1)
			m_view.RemovePage(nActivePage);
		else
			::MessageBeep((UINT)-1);

		return 0;
	}

	LRESULT OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_view.RemoveAllPages();

		return 0;
	}

LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_view.SetActivePage(nPage);

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
		//OpenTodo();
		//this->PostMessage(WM_ICON,IDR_MAINFRAME,WM_RBUTTONUP);
	}
	else if (wParam == uiACCELAR_ID_OPENNOTIFY)
	{
		//OpenTask();
	}

	return 0;
}
////用此函数todo，避免多次启动todo实例。使用快捷键的时候很容易导致这种问题。
//void OpenTodo()
//{
//	DoModalOnce<DialogToDo>();
//}
////用此函数todo，避免多次启动todo实例。使用快捷键的时候很容易导致这种问题。
//void OpenTask()
//{
//	DoModalOnce<CTaskListDialog>();
//}
//
////模板函数：T_dlg类型的对话框同时只能domodal一个
////再次调用的时候，如果发现该对话框已经存在，则闪烁窗口提示
//template<class T_dlg>
//void DoModalOnce()
//{
//	static T_dlg *pDlg  = NULL;
//	if (pDlg==NULL)
//	{
//		T_dlg dlg;
//		pDlg = &dlg;
//		dlg.DoModal();
//		pDlg = NULL;
//	}
//	else
//	{
//		pDlg->ShowWindow(SW_SHOWNORMAL);
//		pDlg->BringWindowToTop();
//		pDlg->FlashWindow(TRUE);
//	}
//}

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
};
