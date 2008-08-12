#pragma once
#include <atltime.h>
#include "./ListCtrl/ListCtrl.h"

class DialogToDo :
	public CDialogImpl<DialogToDo>
{
public:
	DialogToDo(void);
	~DialogToDo(void);

	enum { IDD = IDD_DLG_TODO };

	BEGIN_MSG_MAP(DialogToDo)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		//COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		//COMMAND_ID_HANDLER(IDC_THEME, OnTheme)
		NOTIFY_HANDLER_EX(IDC_LIST_TODO, LCN_SELECTED, OnUserDataSelected)
		//NOTIFY_HANDLER_EX(IDC_USERLIST, LCN_SELECTED, OnUserListSelected)
		//NOTIFY_HANDLER_EX(IDC_LISTCTRL, LCN_HYPERLINK, OnListHyperLink)
		REFLECT_NOTIFICATIONS()
		COMMAND_HANDLER(ID_ADD_TODO, BN_CLICKED, OnBnClickedAddTodo)
		COMMAND_HANDLER(ID_SAVE, BN_CLICKED, OnBnClickedSave)
	END_MSG_MAP()

	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUserDataSelected( LPNMHDR lpNMHDR );
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CListCtrl m_listTodo;
	CListArray<CString> m_aListPriority;;
	CListArray<CString> m_aListState;
public:
	LRESULT OnBnClickedAddTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
