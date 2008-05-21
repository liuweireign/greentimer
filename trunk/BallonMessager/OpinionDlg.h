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

		char hour[16];
		char min[16];

		GetDefaultFillTime(hour, min);

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
		CEdit WndInput = GetDlgItem(IDC_EDT_INPUT);
		TCHAR strMsg[1024];

		CEdit WndHour = GetDlgItem(IDC_HOUR);
		CEdit WndMin = GetDlgItem(IDC_MINUTE);

		TCHAR strHour[3]={0};
		TCHAR strMin[3]={0};

		WndHour.GetWindowText(strHour,3);
		WndMin.GetWindowText(strMin,3);
		WndInput.GetWindowText(strMsg,1024);

		int iHour = atoi(strHour);
		int iMin = atoi(strMin);

		//检查用户是不是忘记输入提示信息了
		ATL::CString strTrimpedMsg = strMsg;

		//只输入空格不算 :)
		strTrimpedMsg.Trim();

		//没有填提示消息,或里面是我们自动填充的消息,不行
		if (strTrimpedMsg.GetLength()==0 || strTrimpedMsg == ATL::CString("请在这里输入提示信息"))
		{
			MessageBox("您忘记在提示信息处输入提示信息了。","您忘了输入提示信息",MB_OK);
			WndInput.SetFocus();
			WndInput.SetWindowText("请在这里输入提示信息"); 
			WndInput.SetSel(0, -1);
		}
		//没有填小时数,不行
		else if (strlen(strHour) == 0)
		{
			MessageBox("应该啥时候提醒您做这件事呀？小时数为空。","您忘记了输入小时数",MB_OK);
			WndHour.SetFocus();

			char hour[16];
			char min[16];

			GetDefaultFillTime(hour, min);

			WndHour.SetWindowText(hour);
			//WndMin.SetWindowText(min);
			WndHour.SetSel(0,-1);
		}
		//小时数不符合要求,不行
		else if (iHour > 23)
		{
			MessageBox("小时数不应该大于23，小时数应该在0-23之间。","楼主，您搞错时间了吧？",MB_OK);
			WndHour.SetFocus();
			WndHour.SetSel(0,-1);
		}
		//没有填分钟数,不行
		else if (strlen(strMin) == 0)
		{
			MessageBox("应该啥时候提醒您做这件事呀？分钟数为空。","您忘了输入分钟数",MB_OK);
			WndMin.SetFocus();

			char hour[16];
			char min[16];

			GetDefaultFillTime(hour, min);

			WndMin.SetWindowText(min);
			//WndHour.SetWindowText(hour);

			WndMin.SetSel(0,-1);
		}
		//分钟数不符合要求,不行
		else if (iMin > 59)
		{
			MessageBox("分钟数应该在0-59之间。","楼主，您搞错时间了吧？",MB_OK);
			WndMin.SetFocus();
			WndMin.SetSel(0,-1);
		}
		//似乎都没有问题
		else
		{
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
						strTrimpedMsg, 
						strFileName); 
					EndDialog(wID);
				}
			}
			//以前没有为现在这个时间设定提醒，那就直接添加吧。
			else
			{
				WritePrivateProfileString ("tips", 
					strTime, 
					strTrimpedMsg, 
					strFileName); 
				EndDialog(wID);
			}
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

	//计算默认填充到设置框的时间
	// *hour 和 *min指向的buffer要有一定的空间.
	void GetDefaultFillTime(char *hour, char *min)
	{
		//当前时间,用来作初始值
		CTime tm = CTime::GetCurrentTime();
		int iNowHour = tm.GetHour();
		int iNowMin = tm.GetMinute();

		int iSetHour = iNowHour;
		int iSetMin = iNowMin + 10; //提前十分钟，GreenTimer认为：没有用户会为当前时间设定提醒的
		if (iSetMin > 59)
		{
			iSetMin -= 60;
			iSetHour += 1;
		}

		//char hour[16];
		itoa(iSetHour,hour,10);

		//char min[16];
		itoa(iSetMin,min,10);
	}
};
