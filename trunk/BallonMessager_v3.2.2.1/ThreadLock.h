#pragma once
//#include <afxmt.h>
#include "Windows.h"

class CThreadLock
{
public:
	CThreadLock(void);
	~CThreadLock(void);
	inline void Lock(void) { EnterCriticalSection(&m_cs); };
	inline void UnLock(void) { LeaveCriticalSection(&m_cs); };
	inline CRITICAL_SECTION &GetRawCS()
	{
		return m_cs;
	}
private:
	CRITICAL_SECTION m_cs;
};

// 堆栈锁，析构时自动解锁，便于使用
class CRangeLock
{
public:
	CRangeLock(CRITICAL_SECTION &cs);

	CRangeLock(CThreadLock &lock);

	~CRangeLock();

protected:
	CRITICAL_SECTION &m_cs;
};

#define LOCK_THIS_RANGE(lock) CRangeLock tmp_range_lock_(lock)
