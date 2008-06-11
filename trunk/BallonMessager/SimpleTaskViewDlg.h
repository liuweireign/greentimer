// SimpleTaskViewDlg.h : CSimpleTaskViewDlg 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>
#include <atlctrls.h>
#include "cwebpage/cwebpage.h"
#include "TaskDB.h"

//HTML文档模板
static const TCHAR *TASKLIST_HTML_TEMPLATE = "<html><head></head><BODY leftmargin=0 topmargin=0 bgcolor=#FFFFFF>"
"<TABLE id=""Table1"" cellSpacing=""2"" cellPadding=""2"" width=""100%%"" bgColor=""#ccffcc"" border=""0"">"
"<TR>"
"<TD width=""22"" bgColor=""#66cc33""><FONT face=""宋体"">ID</FONT></TD>"
"<TD width=""80"" bgColor=""#66cc33""><FONT face=""宋体"">提示类型</FONT></TD>"
"<TD width=""55"" bgColor=""#66cc33""><FONT face=""宋体"">时间</FONT></TD>"
"<TD bgColor=""#66cc33""><FONT face=""宋体"">提示语句</FONT></TD>"
"</TR>"
"%s"		//任务描述行在这里插入
"</TABLE>"
"</BODY>"
"</html>";

//任务描述模板，与TASKLIST_HTML_TEMPLATE配合使用
static const TCHAR *TASK_TMPPLATE = 
"<TR>"
"<TD width=""22"">%d</TD>"
"<TD width=""80"">%s</TD>"
"<TD width=""55"">%s</TD>"
"<TD>%s</TD>"
"</TR>";

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
	for (int i=0;i<vecId.size();i++)
	{
		ITask task;
		if(g_TaskDB.GetTask(vecId[i],task))
		{
			strTasks += GetTaskDesc(task);
		}
	}

	ATL::CString strHtm;
	strHtm.Format(TASKLIST_HTML_TEMPLATE,strTasks);
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
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CSimpleTaskViewDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		//CEdit edit(GetDlgItem(IDC_EDT_TASKLIST));
		////edit.SetWindowText(GetTaskListDesc());
		//edit.ShowWindow(SW_HIDE);

		EmbedBrowserObject(m_hWnd);

		//CRect rt
		//ResizeBrowser(m_hWnd,500,500);
		DisplayHTMLStr(m_hWnd,GetTaskListDesc());

		return 1;  // 使系统设置焦点
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
};


