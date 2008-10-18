// DataPlotDlg.cpp : CDataPlotDlg µÄÊµÏÖ

#include "stdafx.h"
#include "DataPlotDlg.h"


// CDataPlotDlg

LRESULT CDataPlotDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rt;
	GetClientRect(&rt);
	HDC hDC = GetDC();
	
	int dataWidth = (*m_mapData.rbegin()).first - (*m_mapData.begin()).first;
	
	std::map<int,int>::iterator it = m_mapData.begin();
	for (;it!=m_mapData.end();it++)
	{
		int x = rt.Width()*(*it).first/dataWidth;
		::LineTo(hDC,x,(*it).second);
	}

	return 0;
}
