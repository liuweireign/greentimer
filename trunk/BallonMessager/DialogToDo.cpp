#include "StdAfx.h"
#include "resource.h"
#include ".\dialogtodo.h"

#include "ToDoTask.h"
using namespace std;

DialogToDo::DialogToDo(void)
{
	m_bHideFinished = true;
}

DialogToDo::~DialogToDo(void)
{
}

LRESULT DialogToDo::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (SaveData()==S_OK)
	{
		EndDialog(wID);
	}

	return 0;
}

LRESULT DialogToDo::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);

	return 0;
}

LRESULT DialogToDo::OnUserDataSelected( LPNMHDR lpNMHDR )
{
	CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
	ATLTRACE("EDIT:%d,%d\n",pListNotify->m_nItem,pListNotify->m_nSubItem);
	return S_OK;
}

LRESULT DialogToDo::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	m_listTodo.SubclassWindow( GetDlgItem( IDC_LIST_TODO ) );
	m_listTodo.AddColumn(_T("任务"),300,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_listTodo.AddColumn(_T("优先级"),50);
	m_listTodo.AddColumn(_T("状态"),50);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_listTodo.AddColumn(_T("备注"),100,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);

	m_aListPriority.Add(_T("1"));
	m_aListPriority.Add(_T("2"));
	m_aListPriority.Add(_T("3"));
	m_aListPriority.Add(_T("4"));

	m_aListState.Add(_T("未开始"));
	m_aListState.Add(_T("工作中"));
	m_aListState.Add(_T("暂停"));
	m_aListState.Add(_T("已完成"));
	m_aListState.Add(_T("已取消"));

	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	//m_bHideTimeOut = btn.GetCheck();
	btn.SetCheck(m_bHideFinished);

	ReloadTodos();
	return S_OK;
}
LRESULT DialogToDo::OnBnClickedAddTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int id = g_todoSet.AddToDo();

	int iItem = m_listTodo.AddItem(_T("新任务"));
	ATLASSERT(id!=ToDoTask::ERROR_TASKID);
	m_listTodo.SetItemData(iItem,(DWORD)id);
	m_listTodo.SetItemFormat(iItem,1,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListPriority);
	m_listTodo.SetItemComboIndex(iItem,1,1);

	m_listTodo.SetItemFormat(iItem,2,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListState);
	m_listTodo.SetItemComboIndex(iItem,2,1);

	m_listTodo.SetFocus();
	m_listTodo.SetFocusItem(iItem,0);
	m_listTodo.SelectItem(iItem);
	m_listTodo.EditItem(iItem,0);
	return 0;
}

LRESULT DialogToDo::OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return SaveData();
}

LRESULT DialogToDo::SaveData()
{
	for(int i=0;i<m_listTodo.GetItemCount();i++)
	{
		DWORD id;
		m_listTodo.GetItemData(i,id);
		ToDoTask todo = g_todoSet.GetToDo(id);
		if (todo.id!=id)
		{
			ATLASSERT(ToDoTask::ERROR_TASKID==todo.id);
			continue;
		}

		todo.strTask = m_listTodo.GetItemText(i,0).GetBuffer(0);

		todo.priority = (ToDoTask::TaskPriority)m_aListPriority.Find(m_listTodo.GetItemText(i,1));
		todo.state = (ToDoTask::TaskState)m_aListState.Find(m_listTodo.GetItemText(i,2));
		todo.strRemark = m_listTodo.GetItemText(i,3).GetBuffer(0);

		if(!g_todoSet.UpdateToDo(todo))
		{
			MessageBox("保存数据时候出现未知错误。");
			return 0;
		}
	}

	g_todoSet.Save();

	return 0;
}
LRESULT DialogToDo::OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (SaveData()!=S_OK)
	{
		return S_FALSE;
	}
	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	m_bHideFinished = btn.GetCheck();
	ReloadTodos();

	return 0;
}

LRESULT DialogToDo::ReloadTodos()
{
	if(!g_todoSet.Load())
	{
		MessageBox("数据加载错误！");
		return S_FALSE;
	}

	m_listTodo.SetRedraw(FALSE);
	m_listTodo.DeleteAllItems();

	set<int> setID;
	g_todoSet.GetTodoList(setID);
	set<int>::iterator it = setID.begin();
	for (;it!=setID.end();it++)
	{
		ToDoTask todo = g_todoSet.GetToDo(*it);
		if (m_bHideFinished && todo.state==ToDoTask::TS_FINISHED)
		{
			continue;
		}
		int iItem = m_listTodo.AddItem(todo.strTask.c_str());
		ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);
		m_listTodo.SetItemData(iItem,(DWORD)todo.id);
		m_listTodo.SetItemFormat(iItem,1,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListPriority);
		m_listTodo.SetItemComboIndex(iItem,1,todo.priority);
		m_listTodo.SetItemColours(iItem,1,RGB(todo.priority*30+100,todo.priority*30+100,255),RGB(0,0,0));

		m_listTodo.SetItemFormat(iItem,2,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListState);
		m_listTodo.SetItemComboIndex(iItem,2,todo.state);

		m_listTodo.SetItemText(iItem,3,todo.strRemark.c_str());
	}
	m_listTodo.SortItems(1,true);
	m_listTodo.SetRedraw(TRUE);
	return S_OK;
}
LRESULT DialogToDo::OnLvnItemchangedListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}

LRESULT DialogToDo::OnBnClickedAddDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CListArray < int > aSelectedItems;
	m_listTodo.GetSelectedItems(aSelectedItems);
	for (int i=0;i<aSelectedItems.GetSize();i++)
	{
		DWORD id;
		if(!m_listTodo.GetItemData(aSelectedItems[i],id))
		{
			MessageBox("删除列表失败！");
			return S_FALSE;
		}
		if(!g_todoSet.DeleteToDo(id))
		{
			MessageBox("删除列表失败！");
			return S_FALSE;
		}
	}
	g_todoSet.Save();
	ReloadTodos();
	return 0;
}

LRESULT DialogToDo::OnLvnKeydownListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}

LRESULT DialogToDo::OnLvnItemModified(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	SaveData();
	return 0;
}