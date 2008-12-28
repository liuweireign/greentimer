// GreenKnowView.h : interface of the CGreenKnowView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CGreenKnowView : public CDialogImpl<CGreenKnowView>
{
public:
	enum { IDD = IDD_GREENKNOW_FORM };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_MSG_MAP(CGreenKnowView)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};
