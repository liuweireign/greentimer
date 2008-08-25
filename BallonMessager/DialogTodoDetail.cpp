// DialogTodoDetail.cpp : CDialogTodoDetail 的实现

#include "stdafx.h"
#include "DialogTodoDetail.h"
#include "ToDoTask.h"
#include ".\dialogtododetail.h"

// CDialogTodoDetail

LRESULT CDialogTodoDetail::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CDialogTodoDetail>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	ToDoTask todo = g_todoSet.GetToDo(m_id);
	if (todo.id==ToDoTask::ERROR_TASKID)
	{
		MessageBox("任务不存在！","出错了！");
		return 1;
	}
	SetWindowText((todo.strTask + _T(" - 备注")).c_str());
	SetDlgItemText(IDC_EDT_NAME,todo.strTask.c_str());
	SetDlgItemText(IDC_EDT_REMARK,todo.strRemark.c_str());

	return 1;  // 使系统设置焦点
}

LRESULT CDialogTodoDetail::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	ToDoTask todo = g_todoSet.GetToDo(m_id);
	CString strTmp;
	GetDlgItemText(IDC_EDT_NAME,strTmp);
	todo.strTask = strTmp.GetBuffer(0);
	GetDlgItemText(IDC_EDT_REMARK,strTmp);
	todo.strRemark = strTmp.GetBuffer(0);

	g_todoSet.UpdateToDo(todo);

	EndDialog(wID);
	return 0;
}

LRESULT CDialogTodoDetail::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}
LRESULT CDialogTodoDetail::OnBnClickedSetNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
