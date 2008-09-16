#ifdef BALLTRACK_EXPORTS
#define BALLTRACK_DLL_EXPORTED __declspec(dllexport)
#else
#define BALLTRACK_DLL_EXPORTED __declspec(dllimport)
#endif

#ifndef __BALLTRACK_H
#define __BALLTRACK_H

extern "C"
{
	BALLTRACK_DLL_EXPORTED int Init(int iWidth,int iHeight);
	BALLTRACK_DLL_EXPORTED int Uninit();
	BALLTRACK_DLL_EXPORTED void OnBallPos(int x,int y,int d);
};

#endif //#ifndef __SCREENSNAPER_H