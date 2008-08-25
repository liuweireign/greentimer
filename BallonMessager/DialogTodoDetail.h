// DialogTodoDetail.h : CDialogTodoDetail 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>


// CDialogTodoDetail

class CDialogTodoDetail : 
	public CAxDialogImpl<CDialogTodoDetail>
{
public:
	CDialogTodoDetail(int iTodoID)
	{
		m_id = iTodoID;
	}

	~CDialogTodoDetail()
	{
	}

	enum { IDD = IDD_DIALOGTODODETAIL };

BEGIN_MSG_MAP(CDialogTodoDetail)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(ID_SET_NOTIFY, BN_CLICKED, OnBnClickedSetNotify)
	CHAIN_MSG_MAP(CAxDialogImpl<CDialogTodoDetail>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int m_id;

	CEdit m_edtName;
	CEdit m_edtDetail;
public:
	LRESULT OnBnClickedSetNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


