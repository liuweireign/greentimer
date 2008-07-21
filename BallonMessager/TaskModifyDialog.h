// TaskModifyDialog.h : CTaskModifyDialog 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>


// CTaskModifyDialog

class CTaskModifyDialog : 
	public CAxDialogImpl<CTaskModifyDialog>
{
public:
	CTaskModifyDialog(int taskid)
	{
		ATLASSERT(taskid>0);
		m_taskid = taskid;
	}

	~CTaskModifyDialog()
	{
	}

	enum { IDD = IDD_TASKMODIFYDIALOG };

BEGIN_MSG_MAP(CTaskModifyDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CTaskModifyDialog>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int m_taskid;

	CEdit m_edtID;
	CEdit m_edtType;
	CEdit m_edtTime;
	CEdit m_edtTips;
};


