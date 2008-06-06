#include "StdAfx.h"
#include "ThreadLock.h"
//#include "KPrintTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThreadLock::CThreadLock(void)
{
	InitializeCriticalSection(&m_cs);
}

CThreadLock::~CThreadLock(void)
{
	DeleteCriticalSection(&m_cs);
}


//////////////////////////////////////////////////////////////////////////
CRangeLock::CRangeLock(CRITICAL_SECTION &cs)
: m_cs(cs)
{
	//PRINT_MIN_TIME_RANGE("WAIT_LOCK_TIME",100);
	EnterCriticalSection(&m_cs);
}

CRangeLock::CRangeLock(CThreadLock &lock) : m_cs(lock.GetRawCS())
{
	//PRINT_MIN_TIME_RANGE("WAIT_LOCK_TIME",100);
	EnterCriticalSection(&m_cs);
}

CRangeLock::~CRangeLock()
{
	LeaveCriticalSection(&m_cs);
}