#include "StdAfx.h"
#include "resource.h"
#include ".\dialogtodo.h"

DialogToDo::DialogToDo(void)
{
}

DialogToDo::~DialogToDo(void)
{
}

LRESULT DialogToDo::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

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

	CListArray<CString> aListPriority;
	aListPriority.Add(_T("最优先"));
	aListPriority.Add(_T("优先"));
	aListPriority.Add(_T("普通"));
	aListPriority.Add(_T("低优先"));
	CListArray<CString> aListState;
	aListState.Add(_T("未开始"));
	aListState.Add(_T("工作中"));
	aListState.Add(_T("暂停"));
	aListState.Add(_T("已完成"));
	aListState.Add(_T("已取消"));
	for (int i=0;i<10;i++)
	{
		int iItem = m_listTodo.AddItem("任务:");
		m_listTodo.SetItemFormat(i,1,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,aListPriority);
		m_listTodo.SetItemComboIndex(i,1,2);

		m_listTodo.SetItemFormat(i,2,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,aListState);
		m_listTodo.SetItemComboIndex(i,2,0);
	}

	return S_OK;
}