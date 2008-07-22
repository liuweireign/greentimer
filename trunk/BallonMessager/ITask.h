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
		//TT_DATE = 2,		//在特定的日期提醒一次（精确度为天）
		//TT_WEEKLY = 3,		//每周一次
		//TT_MONTHLY = 4,		//每月一次
		//TT_YEARLY = 5,		//每年一次
	};
	static TCHAR *GetTaskTypeStr(TaskType type);
	static TaskType GetTaskType(const string &strName);
	static void GetTaskTypes(vector<string> &vecTypes);

	ATL::CString GetTimeStr() const
	{
		if (Type==TT_DAILY)
		{
			return TaskTime.Format("%H:%M");
		}
		return TaskTime.Format("%y-%m-%d %H:%M:%S");
	}
	ATL::CString GetIDStr() const
	{
		ATL::CString strID;
		strID.Format("%d",Id);
		return strID;
	}

	int Id;
	TaskType Type;	
	CTime TaskTime;	    //任务执行时间
	CTime LastRunTime;  //上次执行时间（为0代表从未执行过）
	CTime CreateTime;	//创建时间
	ATL::CString Tip;	//提示内容

	bool IsTimeNow();
	bool operator == (const ITask &task);
};