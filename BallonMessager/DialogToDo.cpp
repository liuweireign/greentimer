#include "StdAfx.h"
#include "resource.h"
#include ".\dialogtodo.h"
#include "DialogTodoDetail.h"
#include "GlobeFuns.h"

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
	//if (SaveData()==S_OK)
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

	int iCol = 0;
	m_listTodo.AddColumn(_T("创建"),60);
	m_iColCreateTime = iCol++;
	m_listTodo.AddColumn(_T("任务"),320,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColTitle = iCol++;
	m_listTodo.AddColumn(_T("优先"),50);
	m_iColPriority = iCol++;
	m_listTodo.AddColumn(_T("状态"),50);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_iColState = iCol++;
	m_listTodo.AddColumn(_T("备注"),60);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColRemark = iCol++;

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
	ATLASSERT(id!=ToDoTask::ERROR_TASKID);

	//g_todoSet.Save();
	//ReloadTodos();

	int iItem = AddTodoItem(g_todoSet.GetToDo(id));

	m_listTodo.SetFocus();
	m_listTodo.SetFocusItem(iItem,m_iColTitle);
	m_listTodo.SelectItem(iItem);
	m_listTodo.EditItem(iItem,m_iColTitle);
	return 0;
}

//LRESULT DialogToDo::OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	return SaveData();
//}

LRESULT DialogToDo::SaveData(int iItem)
{
	//for(int i=0;i<m_listTodo.GetItemCount();i++)
	//{
	DWORD id;
	m_listTodo.GetItemData(iItem,id);
	ToDoTask todo = g_todoSet.GetToDo(id);
	if (todo.id!=id)
	{
		ATLASSERT(ToDoTask::ERROR_TASKID==todo.id);
		return 0;
	}

	todo.strTask = m_listTodo.GetItemText(iItem,m_iColTitle).GetBuffer(0);

	todo.priority = (ToDoTask::TaskPriority)m_aListPriority.Find(m_listTodo.GetItemText(iItem,m_iColPriority));
	todo.state = (ToDoTask::TaskState)m_aListState.Find(m_listTodo.GetItemText(iItem,m_iColState));
	//todo.strRemark = m_listTodo.GetItemText(i,3).GetBuffer(0);

	if(!g_todoSet.UpdateToDo(todo))
	{
		MessageBox("保存数据时候出现未知错误。");
		return 0;
	}
	//}

	g_todoSet.Save();

	return 0;
}
LRESULT DialogToDo::OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//if (SaveData()!=S_OK)
	//{
	//	return S_FALSE;
	//}
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
		AddTodoItem(todo);


		//m_listTodo.SetItemText(iItem,3,todo.strRemark.c_str());
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
	//如果更改的是任务状态或者任务优先级，刷新一下。
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem<0)
	{
		return 0;
	}

	//保存数据
	SaveData(pNMLV->iItem);

	//刷新界面
	if (m_iColPriority==pNMLV->iSubItem || m_iColState==pNMLV->iSubItem)
	{
		UpdateItem(pNMLV->iItem);
	}
	return 0;
}
LRESULT DialogToDo::OnBnClickedEditTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return EditTodo();
}

int DialogToDo::FindItem( int todo_id )
{
	int i=0;
	for(;i<m_listTodo.GetItemCount();i++)
	{
		DWORD dwItemId;
		m_listTodo.GetItemData(i,dwItemId);
		if (dwItemId==todo_id)
		{
			return i;
		}
	}
	return -1;
}

COLORREF GetStateColor(ToDoTask::TaskState state)
{
	switch(state)
	{
		case ToDoTask::TS_NOT_START:
			return RGB(0XCC,0XFF,0XCC);
		case ToDoTask::TS_WORKING:
			return RGB(10,170,10);
		case ToDoTask::TS_STOP:
			return RGB(10,100,10);
		case ToDoTask::TS_CANCEL:
			return RGB(0XCC,0XCC,0XCC);
		case ToDoTask::TS_FINISHED:
			return RGB(0XAA,0XAA,0XAA);

	}
	return RGB(0,0,0);
}

int DialogToDo::AddTodoItem( ToDoTask &todo )
{
	if (m_bHideFinished 
		&& (todo.state==ToDoTask::TS_FINISHED || todo.state==ToDoTask::TS_CANCEL))
	{
		return -1;
	}
	int iItem = m_listTodo.AddItem(GlobeFuns::TimeToFriendlyString(todo.tmCreateTime).GetBuffer(0));
	m_listTodo.SetItemData(iItem,(DWORD)todo.id);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);
	UpdateItem(iItem);

	//m_listTodo.SetItemText(iItem,m_iColTitle,todo.strTask.c_str());

	//m_listTodo.SetItemFormat(iItem,m_iColPriority,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListPriority);
	//m_listTodo.SetItemComboIndex(iItem,m_iColPriority,todo.priority);
	//m_listTodo.SetItemColours(iItem,m_iColPriority,RGB(10,170-todo.priority*15,10),RGB(0,0,0));

	//m_listTodo.SetItemFormat(iItem,m_iColState,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListState);
	//m_listTodo.SetItemComboIndex(iItem,m_iColState,todo.state);
	//m_listTodo.SetItemColours(iItem,m_iColState,GetStateColor(todo.state),RGB(0,0,0));
	return iItem;
}

void DialogToDo::UpdateItem( int iItem )
{
	DWORD dwId;
	m_listTodo.GetItemData(iItem,dwId);
	ToDoTask todo = g_todoSet.GetToDo(dwId);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);

	m_listTodo.SetItemText(iItem,m_iColTitle,todo.strTask.c_str());

	m_listTodo.SetItemFormat(iItem,m_iColPriority,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListPriority);
	m_listTodo.SetItemComboIndex(iItem,m_iColPriority,todo.priority);
	m_listTodo.SetItemColours(iItem,m_iColPriority,RGB(10,170-todo.priority*15,10),RGB(0,0,0));

	m_listTodo.SetItemFormat(iItem,m_iColState,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListState);
	m_listTodo.SetItemComboIndex(iItem,m_iColState,todo.state);
	m_listTodo.SetItemColours(iItem,m_iColState,GetStateColor(todo.state),RGB(0,0,0));

	m_listTodo.SetItemText(iItem,m_iColRemark,todo.strRemark.c_str());
}

LRESULT DialogToDo::OnLvnDbClick( int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/ )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem<0)
	{
		return 0;
	}
	return EditTodo();
}

LRESULT DialogToDo::EditTodo()
{
	CListArray < int > aSelectedItems;
	m_listTodo.GetSelectedItems(aSelectedItems);
	if (aSelectedItems.IsEmpty())
	{
		MessageBox("请选中所要编辑的待办事项。");
		return S_OK;
	}

	DWORD id;
	ATLVERIFY(m_listTodo.GetItemData(aSelectedItems[0],id));
	CDialogTodoDetail dlg(id);
	dlg.DoModal();
	ReloadTodos();
	return S_OK;
}

LRESULT DialogToDo::OnLvnRClick( int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/ )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem<0)
	{
		return 0;
	}
	CMenu menu;
	menu.CreateMenu();
	menu.AppendMenu(0,5,"aa");
	menu.AppendMenu(0,6,"bbb");
	menu.TrackPopupMenu(0,)
	
	return S_OK;
}