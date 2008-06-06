// SimpleTaskViewDlg.h : CSimpleTaskViewDlg 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>
#include <atlctrls.h>

#include "TaskDB.h"

static ATL::CString GetTaskDesc(const ITask &task)
{
	ATL::CString strDesc;
	strDesc.Format("%d \t%d \t%d:%d \t%s",task.Id,task.Type,task.TaskTime.GetHour(),task.TaskTime.GetMinute(),task.Tip);
	return strDesc;
}

static ATL::CString GetTaskListDesc()
{
	ATL::CString strListDesc = "ID \t TYPE \t TIME \t TIP\r\n";
	std::vector<int> vecId;
	g_TaskDB.GetTaskList(vecId);
	for (int i=0;i<vecId.size();i++)
	{
		ITask task;
		if(!g_TaskDB.GetTask(vecId[i],task))
		{
			continue;
		}
		strListDesc += GetTaskDesc(task) + "\r\n";
	}
	return strListDesc;
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
	CHAIN_MSG_MAP(CAxDialogImpl<CSimpleTaskViewDlg>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CSimpleTaskViewDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		CEdit edit(GetDlgItem(IDC_EDT_TASKLIST));
		edit.SetWindowText(GetTaskListDesc());
		return 1;  // 使系统设置焦点
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};


