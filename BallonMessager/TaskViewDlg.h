#pragma once
#include <atltime.h>
#include <string>

#include "./ATL_DHTML/atldhtmldlg.h"

class TaskViewDlg : public CDHtmlDialogImpl <TaskViewDlg>
{
public:
	enum { IDD = IDD_DLG_TASKVIEW };

	BEGIN_DHTML_EVENT_MAP(TaskViewDlg)
		DHTML_EVENT_ONCLICK(_T("elementid1"), OnClick)
		//DHTML_EVENT_ONMOUSEMOVE(_T("elementid2"), OnMouseMove)
		//DHTML_EVENT_ELEMENT(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("elementid"), OnMouseOver) // id为elementid的元素事件响应
		//DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("myclass"), OnMouseOver) // class为myclass的元素事件响应
		//DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("div"), OnMouseOver) // 所有DIV元素的onmouseover事件响应
		//DHTML_EVENT_AXCONTROL(controlMethodDISPID, _T("objectid"), OnControlMethod) // ActiveX控件事件响应
	END_DHTML_EVENT_MAP() 

	BEGIN_MSG_MAP(TaskViewDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
	END_MSG_MAP()

	HRESULT OnClick(IHTMLElement*)
	{
		return S_OK;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_nHtmlResID = IDR_HTM_TASKVIEW;
		LoadFromResource(IDR_HTM_TASKVIEW);
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		return 0;
	}
};
