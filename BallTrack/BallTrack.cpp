#include "./stdafx.h"
#include <atlbase.h>
#include <atlwin.h>
#include <commctrl.h>

#include "./BallTrack.h"
#include "BallTracker.h"
#include "MouseCtrler.h"

BallTracker g_BallTracker;

CComModule _Module;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch   (ul_reason_for_call)  
	{  
	case   DLL_PROCESS_ATTACH:
		_Module.Init( NULL, (HINSTANCE)hModule );
		break;
	case   DLL_THREAD_ATTACH:  
		break;
	case   DLL_THREAD_DETACH:  
		break;
	case   DLL_PROCESS_DETACH:
		_Module.Term();
		break;  
	}
    return TRUE;
}

int Init(int iWidth,int iHeight)
{
	g_BallTracker.Init(iWidth,iHeight,new MouseCtrler());
	return 0;
}
int Uninit()
{
	return 0;
}
void OnBallPos(int x,int y,int d)
{
	g_BallTracker.OnBallPos(x,y,d);
}