// SimpleTaskViewDlg.cpp : CSimpleTaskViewDlg µÄÊµÏÖ

#include "stdafx.h"
#include "SimpleTaskViewDlg.h"
#include ".\simpletaskviewdlg.h"


// CSimpleTaskViewDlg

LRESULT CSimpleTaskViewDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rt;
	GetWindowRect(&rt);
	ResizeBrowser(m_hWnd,rt.Width(),rt.Height());
	return 0;
}
