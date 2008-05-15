#pragma once
#include <atltime.h>
#include <string>

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

		//当前时间,用来作初始值
		CTime tm = CTime::GetCurrentTime();
		int iNowHour = tm.GetHour();
		char hour[16];
		itoa(iNowHour,hour,10);

		int iNowMin = tm.GetMinute();
		char min[16];
		itoa(iNowMin,min,10);

		//在对话框启动的时候，给编辑框设定初始值
		CWindow WndHour = GetDlgItem(IDC_HOUR);
		WndHour.SetWindowText(hour);

		CWindow WndMinute = GetDlgItem(IDC_MINUTE);
		WndMinute.SetWindowText(min);

		//限定时间和分钟只能在指定范围
		CUpDownCtrl spn1 = GetDlgItem(IDC_SPIN1);
		spn1.SetRange(0,23);

		CUpDownCtrl spn2 = GetDlgItem(IDC_SPIN2);
		spn2.SetRange(0,59);

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
		CWindow WndInput = GetDlgItem(IDC_EDT_INPUT);
		TCHAR strMsg[1024];

		CWindow WndHour = GetDlgItem(IDC_HOUR);
		CWindow WndMin = GetDlgItem(IDC_MINUTE);

		TCHAR strHour[3]={0};
		TCHAR strMin[3]={0};

		WndHour.GetWindowText(strHour,3);
		WndMin.GetWindowText(strMin,3);
		WndInput.GetWindowText(strMsg,1024);

		WTL::CString strTime = strHour;
		strTime += "-";
		strTime += strMin;

		//确认一下:
		WTL::CString strFileName = GetAppDirectory()+"tips.ini";

		char buffer[1024];
		WTL::CString strEmptyTips = "";
		int iCharsRt = GetPrivateProfileString("tips",strTime,strEmptyTips,buffer,sizeof(buffer),strFileName);

		//以前有没有为现在这个时间设定提醒呢？
		if (iCharsRt != 0)
			//以前已经为指定时间设定提醒,那就要用户确认一下了
		{
			WTL::CString strConfirm;
			strConfirm.Format("您曾经要求在： %s 点 %s 分提醒您：\r\n\r\n\"%s\"\r\n\r\n您要替换这个提醒吗？",strHour,strMin ,buffer);

			if(IDOK==MessageBox(strConfirm,"确认一下",MB_OKCANCEL))
			{
				//WTL::CString strFileName = GetAppDirectory()+"tips.ini";
				WritePrivateProfileString ("tips", 
					strTime, 
					strMsg, 
					strFileName); 
				EndDialog(wID);
			}
		}
		//以前没有为现在这个时间设定提醒，那就直接添加吧。
		else
		{
			WritePrivateProfileString ("tips", 
				strTime, 
				strMsg, 
				strFileName); 
			EndDialog(wID);
		}

		return 0;
	}
	// 取得程序运行的目录（以反斜线\结尾）
	WTL::CString GetAppDirectory(bool bEndWithBackSlash=true)
	{
		//取系统路径
		char buf[_MAX_PATH];
		::GetModuleFileName(NULL,buf,_MAX_PATH);
		int iIndex = (int)std::string(buf).rfind('\\');
		if(bEndWithBackSlash)
		{
			buf[iIndex+1]='\0';	//截断，获得路径名(保留最后的'\')
		}
		else
		{
			buf[iIndex]='\0';	//截断，获得路径名(去掉最后的'\')
		}
		return (char *)buf;
	}
};
