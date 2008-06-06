#pragma once
#include <atltime.h>

//抽象任务
class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	enum TaskType{
		TT_ONCE = 0,		//仅运行一次
		TT_DAILY = 1,		//每天运行（精确度为分钟）
		TT_DATE = 2,		//在特定的日期提醒一次（精确度为天）
		TT_WEEKLY = 3,		//每周一次
		TT_MONTHLY = 4,		//每月一次
		TT_YEARLY = 5,		//每年一次
	};

	int Id;
	int Type;
	CTime TaskTime;
	CTime LastRunTime;
	CTime CreateTime;
	ATL::CString Tip;

	bool IsTimeNow();
};

//每日运行的任务
class TaskDaily
{
public:
	int Hour;
	int Minute;
	ATL::CString strTip;
};

////仅运行一次的任务
//class TaskOnce
//{
//public:
//	time_t Time;
//	BOOL IsRuned;
//	CString strTip;
//};