#include "StdAfx.h"
#include ".\tselfstart.h"

using namespace std;

TSelfStart::TSelfStart(const std::string &strName,const std::string &strPath)
{
	m_strPath = strPath;
	m_strName = strName;
}

TSelfStart::~TSelfStart(void)
{
}

bool TSelfStart::SetSelfStart()
{
	HKEY   hRegKey;   
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(RegOpenKey(HKEY_LOCAL_MACHINE,   str.c_str(),   &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}  
	if(::RegSetValueEx(   hRegKey,  
		m_strName.c_str(),  
		0,  
		REG_SZ,  
		(const BYTE *)m_strPath.data(),  
		m_strPath.size()   )   !=   ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);
	return true;
}

bool TSelfStart::IsSelfStart()
{
	HKEY   hRegKey;   
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(RegOpenKey(HKEY_LOCAL_MACHINE,   str.c_str(),   &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}
	char buf[1024];
	DWORD ibufSize = sizeof(buf);
	DWORD iType = REG_SZ;
	if(ERROR_SUCCESS!=
		::RegQueryValueEx(hRegKey, m_strName.c_str(), NULL,&iType,(LPBYTE)buf,(LPDWORD)&ibufSize)
		)
	{
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);
	return m_strPath==buf;
}

bool TSelfStart::RemoveSelfStart()
{
	HKEY   hRegKey;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,   str.c_str(), 0, KEY_ALL_ACCESS,  &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}
	// Jame Yu: This is not needed anymore.
	//if(::RegSetValueEx(   hRegKey,  
	//	m_strName.c_str(),  
	//	0,  
	//	REG_SZ,  
	//	NULL,  
	//	0)   !=   ERROR_SUCCESS)
	//{
	//	RegCloseKey(hRegKey);
	//	return false;
	//}
	if(::RegDeleteValue(hRegKey,m_strName.c_str())!=ERROR_SUCCESS)
	{
		DWORD iErr = GetLastError();
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);
	return true;
}