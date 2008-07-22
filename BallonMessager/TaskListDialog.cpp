// TaskListDialog.cpp : CTaskListDialog 的实现

#include "stdafx.h"
#include <algorithm>

#include "TaskListDialog.h"
#include ".\tasklistdialog.h"
#include "TaskDB.h"

#include ".\OpinionDlg.h"
#include "GlobeFuns.h"
#include "TaskModifyDialog.h"

// CTaskListDialog

LRESULT CTaskListDialog::OnBnClickedBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CTaskModifyDialog dlg(0);	//ID为零表示创建新任务
	if(IDOK!=dlg.DoModal())
	{
		return 0;
	}
	ReloadTasks();
	return 0;
}

LRESULT CTaskListDialog::OnBnClickedBtnDel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	for (int i=0;i<(int)vecTask.size();i++)
	{
		g_TaskDB.RemoveTask(vecTask[i]);
	}
	g_TaskDB.SaveToDB();
	ReloadTasks();
	return 0;
}

LRESULT CTaskListDialog::OnBnClickedBtnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	if (vecTask.empty())
	{
		return 0;
	}
	//if (vecTask.size()>1)
	//{
	//	MessageBox(_T("您同时指定了多个任务。要对任务进行编辑，请只选择"));
	//	return 0;
	//}
	CTaskModifyDialog dlg(vecTask[0]);
	if(IDOK!=dlg.DoModal())
	{
		return 0;
	}
	ReloadTasks();
	return 0;
}

//class ITaskSort
//{
//public:
//	bool operator()(const ITask *left,const ITask *right)   
//	{
//		if (left->Type!=right->Type)
//		{
//			return (int)left->Type<(int)right->Type;
//		}
//	}
//};
bool TaskComp(int idLeft,int idRight)
{
	ITask taskLeft,taskRight;
	g_TaskDB.GetTask(idLeft,taskLeft);
	g_TaskDB.GetTask(idRight,taskRight);
	if (taskLeft.Type==taskRight.Type)
	{
		return taskLeft.TaskTime<taskRight.TaskTime;
	}
	return taskLeft.Type<taskRight.Type;
}

bool IsTaskTimeOut(int id)
{
	ITask task;
	g_TaskDB.GetTask(id,task);
	if (task.Type==ITask::TT_ONCE && task.TaskTime<CTime::GetCurrentTime())
	{
		return true;
	}
	return false;
}

void CTaskListDialog::ReloadTasks()
{
	m_ctlList.SetRedraw(FALSE);

	m_ctlList.DeleteAllItems();

	std::vector<int> vecId;
	g_TaskDB.GetTaskList(vecId);

	//将任务按类型-时间排序
	sort(vecId.begin(),vecId.end(),TaskComp);
	//删除过时的任务
	if (m_bHideTimeOut)
	{
		vecId.erase(remove_if(vecId.begin(),vecId.end(),IsTaskTimeOut),vecId.end());
	}

	for (int i=0;i<(int)vecId.size();i++)
	{
		ITask task;
		if(!g_TaskDB.GetTask(vecId[i],task))
		{
			ATLASSERT(FALSE);
			continue;
		}
		char buf[64];
		int iItemIndex  = m_ctlList.InsertItem(i,itoa(task.Id,buf,10));
		m_ctlList.SetItemData(iItemIndex,task.Id);
		m_ctlList.SetItemText(iItemIndex,1,ITask::GetTaskTypeStr(task.Type));
		m_ctlList.SetItemText(iItemIndex,2,task.GetTimeStr().GetBuffer(0));
		//m_ctlList.SetItemText(iItemIndex,3,task..GetBuffer(0));
		//m_ctlList.SetItemText(iItemIndex,4,task.GetTimeStr().GetBuffer(0));
		m_ctlList.SetItemText(iItemIndex,3,task.Tip.GetBuffer(0));

	}
	m_ctlList.SetRedraw(TRUE);
}

LRESULT CTaskListDialog::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CTaskListDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	CenterWindow();

	m_bHideTimeOut = TRUE;

	m_ctlList = GetDlgItem(IDC_LST_TASK);

	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	//m_bHideTimeOut = btn.GetCheck();
	btn.SetCheck(m_bHideTimeOut);

	ResetTaskList();
	ReloadTasks();
	return 1;  // 使系统设置焦点
}

LRESULT CTaskListDialog::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

LRESULT CTaskListDialog::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	EndDialog(wID);
	return 0;
}

void CTaskListDialog::GetSelectUser( vector<int> &vecSels )
{
	//CListViewCtrl _ctlList(GetDlgItem(IDC_LST_USERLIST));
	for (int i=0;i<m_ctlList.GetItemCount();i++)
	{
		UINT iState = m_ctlList.GetItemState(i,LVIS_SELECTED);
		if(!iState)
		{
			continue;
		}
		int id = m_ctlList.GetItemData(i);
		ATLASSERT(id!=0);
		vecSels.push_back(id);
	}
}

void CTaskListDialog::ResetTaskList()
{
	while(m_ctlList.DeleteColumn(0));	//删除所有列
	int iCol = 0;
	m_ctlList.InsertColumn(iCol++,_T("ID"));          //插入列
	m_ctlList.InsertColumn(iCol++,_T("类型"));
	m_ctlList.InsertColumn(iCol++,_T("时间"));
	//m_ctlList.InsertColumn(iCol++,_T("上次运行"));
	//m_ctlList.InsertColumn(iCol++,_T("创建"));
	m_ctlList.InsertColumn(iCol++,_T("提示语句"));
	CRect rectList;
	m_ctlList.GetClientRect(rectList);                     //获得当前客户区信息
	m_ctlList.SetColumnWidth(0,0);        //设置列的宽度。
	m_ctlList.SetColumnWidth(1,70);
	m_ctlList.SetColumnWidth(2,100);
	m_ctlList.SetColumnWidth(3,rectList.Width()-15-50);
	m_ctlList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
	m_ctlList.DeleteAllItems();
}
LRESULT CTaskListDialog::OnNMDblclkLstTask(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	vector<int> vecTask;
	GetSelectUser(vecTask);
	if (vecTask.empty())
	{
		return 0;
	}
	//if (vecTask.size()>1)
	//{
	//	MessageBox(_T("您同时指定了多个任务。要对任务进行编辑，请只选择"));
	//	return 0;
	//}
	CTaskModifyDialog dlg(vecTask[0]);
	if(IDOK!=dlg.DoModal())
	{
		return 0;
	}
	ReloadTasks();
	return 0;
}

LRESULT CTaskListDialog::OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	m_bHideTimeOut = btn.GetCheck();
	ReloadTasks();

	return 0;
}
