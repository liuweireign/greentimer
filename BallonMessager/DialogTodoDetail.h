// DialogTodoDetail.h : CDialogTodoDetail µÄÉùÃ÷

#pragma once

#include "resource.h"       // Ö÷·ûºÅ
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
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(ID_SET_NOTIFY, BN_CLICKED, OnBnClickedSetNotify)
	COMMAND_HANDLER(ID_SAVE, BN_CLICKED, OnBnClickedSave)
	COMMAND_HANDLER(IDC_CHK_AUTOSAVE, BN_CLICKED, OnBnClickedChkAutosave)
	COMMAND_HANDLER(IDC_EDT_REMARK, EN_CHANGE, OnEnChangeEdtRemark)
	CHAIN_MSG_MAP(CAxDialogImpl<CDialogTodoDetail>)
END_MSG_MAP()
private:
	int m_id;

	CEdit m_edtName;
	CEdit m_edtDetail;
	bool m_bModified;
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedSetNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChkAutosave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdtRemark(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	void SaveToDB();
	void UpdateTitle();
};


