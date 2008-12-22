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
	if(RegOpenKey(HKEY_CURRENT_USER,   str.c_str(),   &hRegKey)   ==   ERROR_SUCCESS)
	{
		if(::RegSetValueEx(   hRegKey,  
			m_strName.c_str(),  
			0,  
			REG_SZ,  
			(const BYTE *)m_strPath.data(),  
			m_strPath.size()   )   ==   ERROR_SUCCESS)
		{
			RegCloseKey(hRegKey);
			return true;
		}
		RegCloseKey(hRegKey);
	}
    MessageBox(NULL,"程序试图为您设定自启动注册表项目但没有成功；\n这可能是由于权限不够造成的。","设定自启动失败!",MB_OK);
	return false;
}

bool TSelfStart::IsSelfStart()
{
	HKEY   hRegKey;
	HKEY   hRegKey1;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	if(RegOpenKey(HKEY_LOCAL_MACHINE,   str.c_str(),   &hRegKey1)  ==   ERROR_SUCCESS)
	{
		//可能用过旧版本,要把旧的注册表项目删除,否则会启动两个实例
		char buf1[1024];
		DWORD ibufSize1 = sizeof(buf1);
		DWORD iType1 = REG_SZ;
		if(ERROR_SUCCESS==
			::RegQueryValueEx(hRegKey1, m_strName.c_str(), NULL,&iType1,(LPBYTE)buf1,(LPDWORD)&ibufSize1)
			)
		{
			if (m_strPath==buf1)
			{
				if(::RegDeleteValue(hRegKey1,m_strName.c_str())!=ERROR_SUCCESS)
				{
					DWORD iErr = GetLastError();
					MessageBox(NULL,"您似乎用过这个软件的旧版本，程序已试图为您删除旧的自启动键值但没有成功；\n请您打开HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n将GreenTimer键值删除。","旧的自启动键值不能删除!",MB_OK);
				}
			}
		}
		RegCloseKey(hRegKey1);
	}
	if(RegOpenKey(HKEY_CURRENT_USER,   str.c_str(),   &hRegKey)   !=   ERROR_SUCCESS)
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

	//2008-12-22日，发现取得的本地路径盘符时而大写时而小写
	//return m_strPath==buf;
	return 0==CString(buf).CompareNoCase(m_strPath.c_str());
}

bool TSelfStart::RemoveSelfStart()
{
	HKEY   hRegKey;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(::RegOpenKeyEx(HKEY_CURRENT_USER,   str.c_str(), 0, KEY_ALL_ACCESS,  &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}
	if(::RegDeleteValue(hRegKey,m_strName.c_str())!=ERROR_SUCCESS)
	{
		DWORD iErr = GetLastError();
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);
	return true;
}