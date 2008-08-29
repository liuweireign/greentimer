// TaskModifyDialog.cpp : CTaskModifyDialog 的实现

#include "stdafx.h"
#include "TaskModifyDialog.h"
#include "TaskDB.h"
#include ".\taskmodifydialog.h"

// CTaskModifyDialog

CTaskModifyDialog::CTaskModifyDialog( int taskid )
{
	//ATLASSERT(taskid>0);	//taskid为零代表创建新任务
	m_taskid = taskid;
}
LRESULT CTaskModifyDialog::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CTaskModifyDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	CenterWindow();

	m_edtID = GetDlgItem(IDC_EDT_ID);
	m_edtTips = GetDlgItem(IDC_EDT_TIPS);
	m_datePicker = GetDlgItem(IDC_DATETIMEPICKER_DATE);
	m_timePicker = GetDlgItem(IDC_DATETIMEPICKER_TIME);

	vector<string> vecTaskTypes;
	ITask::GetTaskTypes(vecTaskTypes);
	ShowToCombox(IDC_CMB_TASKTYPE,vecTaskTypes);

	if (m_taskid==0)	//创建任务
	{
		SetWindowText("创建新提醒");
		m_edtID.SetWindowText("(待创建)");
		m_edtTips.SetWindowText("");
		SYSTEMTIME st;
		CTime::GetCurrentTime().GetAsSystemTime(st);
		m_datePicker.SetSystemTime(GDT_VALID,&st);
		m_timePicker.SetSystemTime(GDT_VALID,&st);
		ComboRollTo(IDC_CMB_TASKTYPE,"每天提醒");
	}
	else	//修改现有任务
	{
		ITask task;
		if(!g_TaskDB.GetTask(m_taskid,task))
		{
			return 0;
		}

		m_edtID.SetWindowText(task.GetIDStr());
		m_edtTips.SetWindowText(task.Tip);

		SYSTEMTIME st;
		task.TaskTime.GetAsSystemTime(st);
		m_datePicker.SetSystemTime(GDT_VALID,&st);
		m_timePicker.SetSystemTime(GDT_VALID,&st);

		ComboRollTo(IDC_CMB_TASKTYPE,ITask::GetTaskTypeStr(task.Type));
	}

	EnableControls();

	return 1;  // 使系统设置焦点
}

LRESULT CTaskModifyDialog::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	if (m_taskid==0)
	{
		ITask task;
		task.CreateTime = CTime::GetCurrentTime();
		task.LastRunTime = 0;
		task.Type = GetSelTaskType();
		task.TaskTime = GetSelTime();
		m_edtTips.GetWindowText(task.Tip);
		if (task.Type==ITask::TT_ONCE && task.TaskTime<CTime::GetCurrentTime())
		{
			MessageBox(_T("很抱歉，我们无法改变历史。请将定时提醒的时刻放在将来。"));
			return 0;
		}
		if (task.Tip.IsEmpty())
		{
			MessageBox(_T("提示语句为空。"));
			return 0;
		}
		g_TaskDB.AddTask(task);
		g_TaskDB.SaveToDB();
	}
	else
	{
		ITask taskOrigin;
		if(!g_TaskDB.GetTask(m_taskid,taskOrigin))
		{
			MessageBox(_T("任务ID出错，数据无法保存，请与开发人员联系"));
			return 0;
		}

		ITask task = taskOrigin;

		task.Type = GetSelTaskType();
		task.TaskTime = GetSelTime();

		if (task.Type==ITask::TT_ONCE && task.TaskTime<CTime::GetCurrentTime())
		{
			MessageBox(_T("很抱歉，我们无法改变历史。请将定时提醒的时刻放在将来。"));
			return 0;
		}
		m_edtTips.GetWindowText(task.Tip);
		if (task==taskOrigin)
		{
			return 0;	//没有做任何更改，不必保存了。
		}
		if (task.Tip.IsEmpty())
		{
			MessageBox(_T("提示语句为空。"));
			return 0;
		}
		task.LastRunTime = 0;	//修改过后，最后运行时间要置零，否则可能不会提示

		g_TaskDB.UpdateTask(task);
		g_TaskDB.SaveToDB();
	}

	EndDialog(wID);

	return 0;
}

LRESULT CTaskModifyDialog::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

void CTaskModifyDialog::ComboRollTo( int idCtrl, const string &strTitle )
{
	CComboBox cmbClass;
	HWND hWnd = GetDlgItem(idCtrl);
	if (hWnd==NULL)
	{
		ATLASSERT(FALSE);
		return;
	}
	cmbClass.Attach(hWnd);

	for (int i=0;i<cmbClass.GetCount();i++)
	{
		//ATL::CString strClass;
		//cmbClass.GetWindowText(strClass);
		TCHAR strClass[1024];
		cmbClass.GetLBText(i,strClass);
		if (0==((ATL::CString)strClass).CompareNoCase(strTitle.c_str()))
		{
			cmbClass.SetTopIndex(i);
			cmbClass.SetCurSel(i);
			return;
		}
	}
}
void CTaskModifyDialog::ShowToCombox( int idCtrl, vector<string> &vecClass )
{
	ATLASSERT(GetDlgItem(idCtrl)!=NULL);
	CComboBox cmbClass;
	cmbClass.Attach(GetDlgItem(idCtrl));
	cmbClass.ResetContent();
	cmbClass.SetTopIndex(0);
	for (unsigned int i=0;i<vecClass.size();i++)
	{
		TCHAR *cls = (TCHAR *)vecClass[i].data();
		ATLTRACE("cmbClass.AddString:%s \n",cls);
		ATLTRACE("cmbClass.AddStringT:%s \n",vecClass[i].c_str());
		cmbClass.AddString(vecClass[i].c_str());
		//cmbClass.InsertString(i,vecClass[i].c_str());
		cmbClass.SetTopIndex(cmbClass.GetCount()-1);
		//cmbClass.SetCaretIndex(cmbClass.GetCount()-1);
		//cmbClass.
	}

	cmbClass.SetTopIndex(0);
	cmbClass.SetCurSel(0);
}
LRESULT CTaskModifyDialog::OnCbnSelchangeCmbTasktype(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EnableControls();
	return 0;
}

void CTaskModifyDialog::EnableControls()
{
	m_datePicker.EnableWindow(GetSelTaskType()==ITask::TT_ONCE);
}

ITask::TaskType CTaskModifyDialog::GetSelTaskType()
{
	TCHAR buf[640];
	::GetWindowText(GetDlgItem(IDC_CMB_TASKTYPE),buf,640);
	return ITask::GetTaskType(buf);
}

ATL::CTime CTaskModifyDialog::GetSelTime()
{
	SYSTEMTIME st;
	m_datePicker.GetSystemTime(&st);
	CTime tmDate = st;

	m_timePicker.GetSystemTime(&st);
	CTime tmTime = st;

	return CTime(tmDate.GetYear(),tmDate.GetMonth(),tmDate.GetDay(),tmTime.GetHour(),tmTime.GetMinute(),tmTime.GetSecond());
}