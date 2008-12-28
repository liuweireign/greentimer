// DialogToDoHistory.h : CDialogToDoHistory 的声明

#pragma once

#include "resource.h"       // 主符号
#include <atlhost.h>

#include "./ListCtrl/ListCtrl.h"
#include "ToDoTask.h"

class CDialogToDoHistory : 
	public CAxDialogImpl<CDialogToDoHistory>,
	public CDialogResize<CDialogToDoHistory>	
{
public:
	CDialogToDoHistory()
	{
	}

	~CDialogToDoHistory()
	{
	}

	enum { IDD = IDD_DIALOGTODOHISTORY };

BEGIN_MSG_MAP(CDialogToDoHistory)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	//NOTIFY_HANDLER(IDC_LIST_TODO, NM_DBLCLK, OnNMDblclkListTodo)
	NOTIFY_HANDLER(IDC_LIST_TODO, LCN_DBLCLICK, OnNMDblclkListTodo)
	COMMAND_HANDLER(ID_EDIT_SETLIVE, BN_CLICKED, OnBnClickedEditSetlive)
	COMMAND_HANDLER(ID_EDIT_TODO, BN_CLICKED, OnBnClickedEditTodo)
	COMMAND_HANDLER(ID_EDIT_DELETEFOREVER, BN_CLICKED, OnBnClickedEditDeleteforever)
	CHAIN_MSG_MAP(CAxDialogImpl<CDialogToDoHistory>)
	CHAIN_MSG_MAP(CDialogResize<CDialogToDoHistory>)
END_MSG_MAP()


BEGIN_DLGRESIZE_MAP(CDialogToDoHistory)
	DLGRESIZE_CONTROL(IDC_LIST_TODO,DLSZ_SIZE_X|DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(ID_EDIT_TODO,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(ID_EDIT_SETLIVE,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(ID_EDIT_DELETEFOREVER,DLSZ_MOVE_X)
END_DLGRESIZE_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	CListCtrl m_listTodoHistory;
	int m_iColCreateTime;
	int m_iColTitle;
	int m_iColPriority;
	int m_iColState;
	int m_iColFinishTime;
	int m_iColTotleHours;
	int m_iColRemark;
private:
	void Refresh();
	int AddTodoItem( ToDoTask &todo );
	void UpdateItem(int iItem);
public:
	LRESULT OnNMDblclkListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedEditSetlive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedEditTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void ShowRemark();
	LRESULT OnBnClickedEditDeleteforever(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


