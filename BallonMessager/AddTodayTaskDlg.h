// AddTodayTaskDlg.h : CAddTodayTaskDlg 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>
#include <atlctrls.h>
#include <atltime.h>
#include <string>

// CAddTodayTaskDlg

class CAddTodayTaskDlg : 
	public CAxDialogImpl<CAddTodayTaskDlg>
{
public:
	CAddTodayTaskDlg()
	{
	}

	~CAddTodayTaskDlg()
	{
	}

	enum { IDD = IDD_ADDTODAYTASKDLG };

BEGIN_MSG_MAP(CAddTodayTaskDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CAddTodayTaskDlg>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CAddTodayTaskDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		m_edtTodayTask.Attach(GetDlgItem(IDC_EDT_TODAYTASK));
		m_edtTodayTask.SetWindowText("10:00 专心工作\r\n"
			"12:00 午餐\r\n"
			"13:00 休息\r\n"
			);
		return 1;  // 使系统设置焦点
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		TCHAR strText[1024*10];
		m_edtTodayTask.GetWindowText(strText,1024*10);
		std::string strTasks(strText);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
private:
	CEdit m_edtTodayTask;
};


