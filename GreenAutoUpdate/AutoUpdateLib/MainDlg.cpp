// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <ATLCTRLS.H> // for CProgressBarCtrl

#include <fstream>
#include "../curl/curl.h"
#include "./NetLogger.h"

using namespace std;

int OnProgress(void *clientp, double dltotal, double
        dlnow, double ultotal, double ulnow)
{
	if(0==dltotal)
	{
		return 0;
	}
	PostMessage((HWND)clientp,WM_PROGRESS,(WPARAM)dlnow,(LPARAM)dltotal);
    return 0;
}

size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream)
{
	//cout<<(char *)ptr<<endl;
	//for(int i=0;i<2*size*nmemb/512;i++)
	//{
	//	cout<<".";
	//}
	//cout<<endl;

	ofstream &ofs = *(ofstream *)stream;
	ofs.write((char *)ptr,size*nmemb);
	return size*nmemb;
}

DWORD WINAPI ThreadDownload(LPVOID s)
{
	HWND wnd = (HWND)s;
	ofstream ofs("c:\\WTL80_sf.exe",ios_base::binary);
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(!curl) {
		return 0;
	}
	NetLogger::WriteLog(50,1,"Start downloading....");

	curl_easy_setopt(curl, CURLOPT_URL, "http://greentimer.googlecode.com/svn/trunk/compile_resource/WTL80_sf.exe");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, wnd);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS,FALSE);

	res = curl_easy_perform(curl);
	if(0!=res)
	{
		NetLogger::WriteLog(50,1,"Download error ");
		return 0;
	}

	/* always cleanup */
	curl_easy_cleanup(curl);
	NetLogger::WriteLog(50,1,"Download is finished....");

	return 0;
}


BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	NetLogger::WriteLog(50,1,"Test program starting....");
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	CProgressBarCtrl progress(GetDlgItem(IDC_PROGRESS_DOWNLOAD));
	progress.SetRange(0,100);
	progress.SetStep(10);

	CreateThread(NULL, 0, ThreadDownload, m_hWnd, 0, NULL);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	NetLogger::WriteLog(50,1,"Test program exit....");

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedBtnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();

	return 0;
}
LRESULT CMainDlg::OnProgressUpdate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	ATLTRACE("progress:%d\n",wParam);
	CProgressBarCtrl progress(GetDlgItem(IDC_PROGRESS_DOWNLOAD));
	progress.SetPos(wParam*100/lParam);
	if(wParam==lParam)
	{
		MessageBox(_T("下载完成！"));
	}
	return 0;
}