#pragma once
#include <atltime.h>
#include <vector>

#include "./ListCtrl/ListCtrl.h"
#include "ToDoTask.h"

class DialogToDo :
	public CDialogImpl<DialogToDo>,
	public CDialogResize<DialogToDo>	
{
public:
	DialogToDo(void);
	~DialogToDo(void);

	enum { IDD = IDD_DLG_TODO };

	BEGIN_MSG_MAP(DialogToDo)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(ID_ADD_TODO, BN_CLICKED, OnBnClickedAddTodo)
		//COMMAND_HANDLER(ID_SAVE, BN_CLICKED, OnBnClickedSave)
		COMMAND_HANDLER(IDC_CHK_HIDEOUTTIME, BN_CLICKED, OnBnClickedChkHideouttime)
		COMMAND_HANDLER(ID_ADD_DELETE, BN_CLICKED, OnBnClickedAddDelete)
		COMMAND_HANDLER(ID_EDIT_TODO, BN_CLICKED, OnBnClickedEditTodo)
		COMMAND_HANDLER(ID_ADD_REFRESH, BN_CLICKED, OnBnClickedAddRefresh)
		COMMAND_HANDLER(ID_ADD_SHOWRECYCLE, BN_CLICKED, OnBnClickedAddShowrecycle)
		COMMAND_HANDLER(ID_ADD_SHOWSTATICS, BN_CLICKED, OnBnClickedAddShowstatics)
		//COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		//COMMAND_ID_HANDLER(IDC_THEME, OnTheme)
		NOTIFY_HANDLER(IDC_LIST_TODO, LVN_KEYDOWN, OnLvnKeydownListTodo)
		NOTIFY_HANDLER(IDC_LIST_TODO, LCN_ENDEDIT, OnLvnItemchangedListTodo)
		NOTIFY_HANDLER_EX(IDC_LIST_TODO, LCN_SELECTED, OnUserDataSelected)
		NOTIFY_HANDLER(IDC_LIST_TODO, LCN_MODIFIED, OnLvnItemModified)
		NOTIFY_HANDLER(IDC_LIST_TODO, LCN_DBLCLICK, OnLvnDbClick)
		NOTIFY_HANDLER(IDC_LIST_TODO, LCN_RIGHTCLICK, OnLvnRClick)
		//NOTIFY_HANDLER_EX(IDC_USERLIST, LCN_SELECTED, OnUserListSelected)
		//NOTIFY_HANDLER_EX(IDC_LISTCTRL, LCN_HYPERLINK, OnListHyperLink)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<DialogToDo>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(DialogToDo)
		DLGRESIZE_CONTROL(IDC_LIST_TODO,DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(ID_ADD_TODO,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_EDIT_TODO,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_ADD_REFRESH,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_ADD_DELETE,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_ADD_SHOWRECYCLE,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_ADD_SHOWSTATICS,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_CHK_HIDEOUTTIME,DLSZ_MOVE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUserDataSelected( LPNMHDR lpNMHDR );
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAddTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLvnItemchangedListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAddDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLvnKeydownListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnLvnItemModified(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedEditTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLvnDbClick(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnLvnRClick(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
private:
	CListCtrl m_listTodo;
	CListArray<CString> m_aListPriority;;
	CListArray<CString> m_aListState;

	//¿ØÖÆÏÔÊ¾ºÍÒþ²ØÄÄÐ©Æ¬¶Î
	static std::vector<BOOL> m_vecPriorityShow;
	static std::vector<BOOL> m_vecStateShow;

	BOOL m_bHideFinished;

	int m_iColID;
	int m_iColCreateTime;
	int m_iColTitle;
	int m_iColPriority;
	int m_iColState;
	int m_iColRemark;
private:
	LRESULT SaveData(int iItem);
	int FindItem(int todo_id);
	LRESULT ReloadTodos();
	int AddTodoItem( ToDoTask &todo );
	void UpdateItem(int iItem);
	LRESULT EditTodo();
	void UpdateCounts();

	bool ShowColMenu(const CPoint &pt,const CListArray<CString> &colData, std::vector<BOOL> &vecSelect);
public:
	LRESULT OnBnClickedAddRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAddShowrecycle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAddShowstatics(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
