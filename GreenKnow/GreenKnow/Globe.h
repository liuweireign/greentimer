#pragma once
#include <string>
#include "DBSetting.h"

class Globe
{
public:
	static DWORD GetHotKeyOpenTask();
	static bool SetHotKeyOpenTask(DWORD val);
	static DWORD GetHotKeyOpenNotify();
	static bool SetHotKeyOpenNotify(DWORD valD);

	static void SaveDlgSizeToDB(CWindow wnd,int id);
	static void ReadDlgSizeFromDB(CWindow wnd,int id);

	static std::string GetDBPath();
	static DBSetting dbSetting;
};
