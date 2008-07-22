// TaskListDialog.h : CTaskListDialog 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlmisc.h>
#include <atlhost.h>
#include <atlctrls.h>

// CTaskListDialog

class CTaskListDialog : 
	public CAxDialogImpl<CTaskListDialog>
{
public:
	CTaskListDialog()
	{
	}

	~CTaskListDialog()
	{
	}

	enum { IDD = IDD_TASKLISTDIALOG };

BEGIN_MSG_MAP(CTaskListDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(ID_BTN_ADD, BN_CLICKED, OnBnClickedBtnAdd)
	COMMAND_HANDLER(ID_BTN_DEL, BN_CLICKED, OnBnClickedBtnDel)
	COMMAND_HANDLER(ID_BTN_EDIT, BN_CLICKED, OnBnClickedBtnEdit)
	NOTIFY_HANDLER(IDC_LST_TASK, NM_DBLCLK, OnNMDblclkLstTask)
	COMMAND_HANDLER(IDC_CHK_HIDEOUTTIME, BN_CLICKED, OnBnClickedChkHideouttime)
	CHAIN_MSG_MAP(CAxDialogImpl<CTaskListDialog>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnBnClickedBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnDel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	CListViewCtrl m_ctlList;
	BOOL m_bHideTimeOut;
private:
	void ReloadTasks();
	void ResetTaskList();
	void GetSelectUser( std::vector<int> &vecSels );
public:
	LRESULT OnNMDblclkLstTask(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


