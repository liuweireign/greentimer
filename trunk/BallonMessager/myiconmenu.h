//MyIconMenu.h

#pragma once

#include "stdafx.h"
#include "ContextMenu.h"

template <class T>
class CMyIconMenu : public CContextMenu<T>
{
public:
	BEGIN_MSG_MAP(CContextMenu)
		COMMAND_ID_HANDLER(ID_RESUME, OnResume)
		COMMAND_ID_HANDLER(ID_QUIT, OnQuit)
	END_MSG_MAP()

	LRESULT OnResume(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		BOOL bVisible = pT->IsWindowVisible();
		pT->ShowWindow(!bVisible);
		//OpenIcon(pT->m_hWnd);
		return 0;
	}

	LRESULT OnQuit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		pT->CloseDialog(0);
		return 0;
	}
};