#pragma once

class COpinionDlg : public CDialogImpl<COpinionDlg>
{
public:
	enum { IDD = IDD_DLG_SETUP };

	BEGIN_MSG_MAP(COpinionDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		//在对话框启动的时候，给编辑框设定初始值
		CWindow Wnd = GetDlgItem(IDC_EDT_INPUT);
		Wnd.SetWindowText(_T("Hello,world!"));
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//当点击“OK”按钮的时候，取出编辑框的值并用对话框显示出来。
		CWindow Wnd = GetDlgItem(IDC_EDT_INPUT);
		TCHAR strText[1024];
		Wnd.GetWindowText(strText,1024);
		WTL::CString str = strText;
		str += "\r\n 按确定关闭对话框。";
		if(IDOK==MessageBox(str,"测试",MB_OKCANCEL))
		{
			EndDialog(wID);
		}
		return 0;
	}
};
