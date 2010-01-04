// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0600
#define _RICHEDIT_VER	0x0200

//
//#include <atlbase.h>
//#include <atlapp.h>
//#include <atlstr.h>
//#include <atlmisc.h>
//
//#define _WTL_NO_CSTRING
//
//#include <atlctrls.h>
//#include <atlwin.h>
//#include <atlcrack.h>
//#include <atlframe.h>
//
//extern CAppModule _Module;

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>

extern CAppModule _Module;

#define _WTL_NO_CSTRING

#include <atlwin.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atltime.h>
#define END_MSG_MAP_EX	END_MSG_MAP

#include <vector>
#include <map>
#include <list>
#include <set>
#include <vector>
using namespace std;

#include "resource.h"

#pragma warning(disable:4996)


//#if defined _M_IX86
//  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_IA64
//  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
