// TaskModifyDialog.cpp : CTaskModifyDialog 的实现

#include "stdafx.h"
#include "TaskModifyDialog.h"
#include "TaskDB.h"
#include ".\taskmodifydialog.h"

// CTaskModifyDialog

LRESULT CTaskModifyDialog::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CTaskModifyDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	m_edtID = GetDlgItem(IDC_EDT_ID);
	m_edtType = GetDlgItem(IDC_EDT_TYPE);
	m_edtTime = GetDlgItem(IDC_EDT_TIME);
	m_edtTips = GetDlgItem(IDC_EDT_TIPS);

	ITask task;
	if(!g_TaskDB.GetTask(m_taskid,task))
	{
		return 0;
	}

	m_edtID.SetWindowText(task.GetIDStr());
	m_edtType.SetWindowText(ITask::GetTaskTypeStr(task.Type));
	m_edtTime.SetWindowText(task.GetTimeStr());
	m_edtTips.SetWindowText(task.Tip);

	return 1;  // 使系统设置焦点
}

LRESULT CTaskModifyDialog::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);

	ITask task;
	if(!g_TaskDB.GetTask(m_taskid,task))
	{
		MessageBox(_T("任务ID出错，数据无法保存，请与开发人员联系"));
		return 0;
	}
	m_edtTips.GetWindowText(task.Tip);
	g_TaskDB.UpdateTask(task);
	g_TaskDB.SaveToDB();

	return 0;
}

LRESULT CTaskModifyDialog::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

LRESULT CTaskModifyDialog::OnBnClickedBtnTimebrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
