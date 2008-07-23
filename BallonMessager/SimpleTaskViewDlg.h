// SimpleTaskViewDlg.h : CSimpleTaskViewDlg 的声明

#pragma once

#include <mshtmlc.h>
#include <atlhost.h>
#include <atlctrls.h>
#include "resource.h"       // 主符号
//#include "cwebpage/cwebpage.h"
#include "TaskDB.h"
#include "OpinionDlg.h"

////HTML文档模板
//static const TCHAR *TASKLIST_HTML_TEMPLATE = "<html><head></head><BODY leftmargin=0 topmargin=0 bgcolor=#FFFFFF>"
//"<TABLE id=""Table1"" cellSpacing=""2"" cellPadding=""2"" width=""100%%"" bgColor=""#ccffcc"" border=""0"">"
//"<TR>"
//"<TD width=""22"" bgColor=""#66cc33""><FONT face=""宋体"">ID</FONT></TD>"
//"<TD width=""80"" bgColor=""#66cc33""><FONT face=""宋体"">提示类型</FONT></TD>"
//"<TD width=""55"" bgColor=""#66cc33""><FONT face=""宋体"">时间</FONT></TD>"
//"<TD bgColor=""#66cc33""><FONT face=""宋体"">提示语句</FONT></TD>"
//"</TR>"
//"%s"		//任务描述行在这里插入
//"</TABLE>"
//"</BODY>"
//"</html>";

//任务描述模板，与IDR_HTM_TASKVIEW配合使用
static const TCHAR *TASK_TMPPLATE = 
"<TR>"
"<TD width=""32"">%d</TD>"
"<TD width=""80"">%s</TD>"
"<TD width=""55"">%s</TD>"
"<TD>%s</TD>"
"</TR>";

const int c_iBtnID = 5;

static ATL::CString LoadHtmlRes(UINT uiHTMLRes)
{
	HRSRC hr = FindResource(NULL,MAKEINTRESOURCE(uiHTMLRes),RT_HTML);
	if (NULL==hr)
	{
		return "";
	}
	HGLOBAL hg = LoadResource(NULL,hr);
	return (TCHAR *)LockResource(hg);
}

static ATL::CString GetTaskDesc(const ITask &task)
{
	//ATL::CString strDesc;
	//strDesc.Format("%d \t%d \t%d:%d \t%s",task.Id,task.Type,task.TaskTime.GetHour(),task.TaskTime.GetMinute(),task.Tip);
	ATL::CString strTask;
	strTask.Format(TASK_TMPPLATE,
		task.Id,
		ITask::GetTaskTypeStr(task.Type),
		task.GetTimeStr(),
		task.Tip);
	return strTask;
}

static ATL::CString GetTaskListDesc()
{
	ATL::CString strTasks;

	std::vector<int> vecId;
	g_TaskDB.GetTaskList(vecId);
	for (int i=0;i<(int)vecId.size();i++)
	{
		ITask task;
		if(g_TaskDB.GetTask(vecId[i],task))
		{
			strTasks += GetTaskDesc(task);
		}
	}

	ATL::CString strHtm;
	//strHtm.Format(TASKLIST_HTML_TEMPLATE,strTasks);

	strHtm.Format(LoadHtmlRes(IDR_HTM_TASKVIEW),strTasks);
	return strHtm;
}
// CSimpleTaskViewDlg

class CSimpleTaskViewDlg : 
	public CAxDialogImpl<CSimpleTaskViewDlg>
{
public:
	CSimpleTaskViewDlg()
	{
	}

	~CSimpleTaskViewDlg()
	{
	}

	enum { IDD = IDD_SIMPLETASKVIEWDLG };

BEGIN_MSG_MAP(CSimpleTaskViewDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(CAxDialogImpl<CSimpleTaskViewDlg>)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_NOTIFY, OnCtrlNotify)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CSimpleTaskViewDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		//DisplayHTMLStr(m_hWnd,LoadHtmlRes(IDR_HTM_TASKVIEW));
		EmbedBrowserObject(m_hWnd);
		InitHtml();
        
		return 1;  // 使系统设置焦点
	}

	void InitHtml()
	{
		//CRect rt
		//ResizeBrowser(m_hWnd,500,500);
		DisplayHTMLStr(m_hWnd,GetTaskListDesc());

		RegisterEvent();
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(wID);
		return 0;
	}


	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		UnEmbedBrowserObject(m_hWnd);
		EndDialog(IDOK);
		return 0;
	}
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	//{
	//	ATLTRACE("%d,%d,%d\n",idCtrl,pnmh->code,pnmh->idFrom);
	//	return 0;
	//}

	LRESULT OnCtrlNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		//ATLTRACE("%d,%d,%d\n",idCtrl,pnmh->code,pnmh->idFrom);
		LPNMHDR pnmh = (LPNMHDR)lParam;
		ATLTRACE("%d,%d,%d\n",wParam,pnmh->code,pnmh->idFrom);
		if (pnmh->hwndFrom == m_hWnd && !pnmh->idFrom)
		{
			// This message is sent from one of our _IDispatchEx's. The NMHDR is really
			// a WEBPARAMS struct, so we can recast it as so. Also, the wParam is the
			// __IDispatchEx object we used to attach to the event that just happened.
			WEBPARAMS		*webParams;
			//_IDispatchEx	*lpDispatch;

			webParams =		(WEBPARAMS *)lParam;
			//lpDispatch = 	(_IDispatchEx *)wParam;

			//if (lpDispatch->id==c_iBtnID)
			//{
			//	OnBtnAddTask();
			//}
			//if (webParams->nmhdr.code==c_iBtnID)	//本应当判断ID的，可是_IDispatchEx结构编译通不过，无法获取
			if (ATL::CString("click") == webParams->eventStr)
			{
				OnBtnAddTask();
			}
		}
		
		//ATLTRACE("%d,%d,%d\n",uMsg,wParam,lParam);
		return 0;
	}
	void OnBtnAddTask()
	{
		COpinionDlg dlg;
		dlg.DoModal();
		InitHtml();
	}
	void RegisterEvent()
	{
		register IHTMLElement *		elem;
		IHTMLDocument2 *			htmlDoc2;
		//IHTMLWindow2 *				htmlWin2;
		VARIANT						varDisp;

		// Wait for the browser to achieve its READYSTATE_COMPLETE (before we
		// try to get the IHTMLDocument2).
		if (WaitOnReadyState(m_hWnd, READYSTATE_COMPLETE, 3000, NULL) != WORS_SUCCESS ||
			GetWebPtrs(m_hWnd, 0, &htmlDoc2))
		{
			ATLASSERT(FALSE);
			return ;
		}

		VariantInit(&varDisp);
		varDisp.vt = VT_DISPATCH;

		if ((elem = GetWebElement(m_hWnd, htmlDoc2, _T("btn_addtask"), 0)) &&
			(varDisp.pdispVal = CreateWebEvtHandler(m_hWnd, htmlDoc2, c_iBtnID, c_iBtnID, (IUnknown *)elem, (void *)&c_iBtnID)))
		{
			elem->put_onclick(varDisp);

		}

		//// Get the IHTMLElement object for the FONT tag (whose id is "testfont")
		//if ((elem = GetWebElement(m_hWnd, htmlDoc2, _T("testfont"), 0)) &&

		//	// Create one of our _IDispatchEx objects associated with this FONT tag. Messages
		//	// about events we "capture" will be sent to our window (by _IDispatchEx's Invoke()).
		//	// Note that we'll store our IHTMLElement object into our _IDispatchEx's "object"
		//	// field. We arbitrarily assign an ID of 1 to this FONT tag's events. We'll use this ID
		//	// in our WM_NOTIFY handling to refer to events from this FONT tag. So, we pass that as
		//	// the ID argument
		//	(varDisp.pdispVal = CreateWebEvtHandler(m_hWnd, htmlDoc2, 0, 1, (IUnknown *)elem, 0)))
		//{
		//	// "Capture" the "mouseover" event. When the user moves the mouse over
		//	// this FONT tag, our _IDispatchEx's Invoke() will be called, and it will
		//	// in turn send a WM_NOTIFY to our window
		//	//elem->lpVtbl->put_onmouseover(elem, varDisp);

		//	// "Capture" the "mouseout" event
		//	elem->lpVtbl->put_onmouseout(elem, varDisp);

		//	// NOTE: We must not VariantClear(&varDisp) or that will deallocate our _IDispatchEx, and
		//	// we don't want to do that now! Also, we don't Release "elem" because we told 
		//	// CreateWebEvtHandler to store that in the returned _IDispatchEx. We'll need it later
		//	// when we receive a WM_NOTIFY message with this _IDispatchEx.
		//}

		//// Now do a similiar thing for the FORM. But in this case we capture the
		//// "submit" event. We also assign this element an ID of 2. We store
		//// an IHTMLFormElement object in the _IDispatchEx's object field. NOTE:
		//// the FORM has an id name of "testform"
		//if ((elem = GetWebElement(m_hWnd, htmlDoc2, _T("testform"), 0)))
		//{
		//	IHTMLFormElement *	htmlForm;

		//	if (!elem->lpVtbl->QueryInterface(elem, &IID_IHTMLFormElement, (void **)&htmlForm))
		//	{
		//		if ((varDisp.pdispVal = CreateWebEvtHandler(m_hWnd, htmlDoc2, 0, 2, (IUnknown *)htmlForm, 0)))
		//			htmlForm->lpVtbl->put_onsubmit(htmlForm, varDisp);
		//	}
		//	else
		//		elem->lpVtbl->Release(elem);
		//}

		//// Capture any "scroll" event. This event is not associated with any particular element
		//// on a web page. So unlike the above, where we get the HTMLElement object for the element,
		//// we don't do that here. Instead, we need to get the IHTMLWindow2 object and use that.
		//// Also note that we arbitrary assign an ID of 3 to these events
		//if (!htmlDoc2->lpVtbl->get_parentWindow(htmlDoc2, &htmlWin2) && htmlWin2 &&

		//	(varDisp.pdispVal = LpCreateWebEvtHandler(m_hWnd, htmlDoc2, 0, 3, (IUnknown *)htmlWin2, 0)))
		//{
		//	htmlWin2->lpVtbl->put_onscroll(htmlWin2, varDisp);
		//}

		//// Capture any "dblclick" event. Again, this is not associated with any particular element.
		//// We arbitrarily assign an ID of 4 to these events
		//if ((varDisp.pdispVal = CreateWebEvtHandler(m_hWnd, htmlDoc2, 0, 4, (IUnknown *)htmlDoc2, 0)))
		//{
		//	htmlDoc2->lpVtbl->put_ondblclick(htmlDoc2, varDisp);
		//}
	}
};


