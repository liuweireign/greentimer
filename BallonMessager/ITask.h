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
	static TCHAR *GetTaskTypeStr(TaskType type)
	{
		switch(type)
		{
		case TT_ONCE:
			return "一次提醒";
		case TT_DAILY:
			return "每天提醒";
		case TT_DATE: 
			return "日期提醒";
		case TT_WEEKLY: 
			return "每周提醒";
		case TT_MONTHLY: 
			return "每月提醒";
		case TT_YEARLY:
			return "每年提醒";
		default:
			return "未知类型";
		}
		return "未知类型";
	}

	ATL::CString GetTimeStr() const
	{
		if (Type==TT_DAILY)
		{
			return TaskTime.Format("%H:%M");
		}
		return TaskTime.Format("%y-%m-%d %H:%M:%S");
	}

	int Id;
	TaskType Type;
	CTime TaskTime;
	CTime LastRunTime;
	CTime CreateTime;
	ATL::CString Tip;

	bool IsTimeNow();
};

//每日运行的任务
//class TaskDaily
//{
//public:
//	int Hour;
//	int Minute;
//	ATL::CString strTip;
//};

////仅运行一次的任务
//class TaskOnce
//{
//public:
//	time_t Time;
//	BOOL IsRuned;
//	CString strTip;
//};