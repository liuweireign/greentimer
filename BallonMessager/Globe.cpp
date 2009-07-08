#include "stdafx.h"
#include "Globe.h"
#include "GlobeFuns.h"

enum SettingKey 
{
	HOTKEY_OPENTASK = 1000,
	HOTKEY_OPENNOTIFY = 1001,

	//这部分字段保留给保存对话框大小使用，其值为DB_ITEM_ID_BASE+IDD，IDD是对话框ID，一般不超过1000
	DB_ITEM_ID_BASE = 10000,
};


//static 
DBSetting Globe::dbSetting(Globe::GetDBPath(),"T_Setting");

std::string Globe::GetDBPath()
{
	return GlobeFuns::GetAppDirectory() + _T("task.db");
}

DWORD Globe::GetHotKeyOpenTask()
{
	long val;
	if(!dbSetting.GetValue(HOTKEY_OPENTASK,val))
	{
		return MAKELONG('G',MOD_CONTROL);
	}
	return val;
}
bool Globe::SetHotKeyOpenTask(DWORD val)
{
	return dbSetting.SetValue(HOTKEY_OPENTASK,val);
}

DWORD Globe::GetHotKeyOpenNotify()
{
	long val;
	if(!dbSetting.GetValue(HOTKEY_OPENNOTIFY,val))
	{
		return MAKELONG('T',MOD_CONTROL);
	}
	return val;
}
bool Globe::SetHotKeyOpenNotify(DWORD val)
{
	return dbSetting.SetValue(HOTKEY_OPENNOTIFY,val);
}

void Globe::SaveDlgSizeToDB( CWindow wnd,int id )
{
	//if (wnd.IsZoomed() || wnd.IsIconic())	//如果是最大化状态，似乎不应当记住窗口大小
	//{
	//	return;
	//}
	CRect rt;
	wnd.GetClientRect(&rt);

	Globe::dbSetting.SetValue(DB_ITEM_ID_BASE+id,rt.Width());
	Globe::dbSetting.SetValue(DB_ITEM_ID_BASE+id+1,rt.Height());
}

void Globe::ReadDlgSizeFromDB( CWindow wnd,int id )
{
	long lWidth,lHeight;
	if(Globe::dbSetting.GetValue(DB_ITEM_ID_BASE+id,lWidth) &&
		Globe::dbSetting.GetValue(DB_ITEM_ID_BASE+id+1,lHeight) )
	{
		wnd.MoveWindow(0,0,lWidth,lHeight);
	}
}