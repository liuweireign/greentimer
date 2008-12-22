#include "stdafx.h"
#include ".\itask.h"

ITask::ITask(void)
{
	LastRunTime = 0;
}

ITask::~ITask(void)
{
}

bool ITask::IsTimeNow()
{

	CTime tmNow = CTime::GetCurrentTime();
	switch(Type)
	{
	case TT_DAILY:
		{
			//每日运行任务，只要时间和分钟正确即可。
			if (tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//避免重复运行。
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,是运行时间
				return true;
			}
		}
		break;
	case TT_ONCE:
		{

			//一次性运行任务。
			if (LastRunTime!=0)	//已经运行过了
			{
				return false;
			}
			if (tmNow>TaskTime)	//若超出时间还没提示，那就得提示
			{
				return true;
			}
		}
	case TT_WEEKLY:
		{
			//每周运行任务
			if (tmNow.GetDayOfWeek()==GetMonthWeek(TaskTime)
				&& tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//避免重复运行。
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,是运行时间
				return true;
			}
		}
	case TT_MONTHLY:
		{
			//每月运行任务
			if (tmNow.GetDay()==GetMonthWeek(TaskTime)
				&& tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//避免重复运行。
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,是运行时间
				return true;
			}

		}
	}
	return false;
}

TCHAR * ITask::GetTaskTypeStr( TaskType type )
{
	switch(type)
	{
	case TT_ONCE:
		return "一次提醒";
	case TT_DAILY:
		return "每天提醒";
		//case TT_DATE: 
		//	return "日期提醒";
	case TT_WEEKLY: 
		return "每周提醒";
	case TT_MONTHLY: 
		return "每月提醒";
		//case TT_YEARLY:
		//	return "每年提醒";
	default:
		return "未知类型";
	}
	return "未知类型";
}

ITask::TaskType ITask::GetTaskType( const string &strName )
{
	if (strName.compare("一次提醒")==0)
	{
		return TT_ONCE;
	}
	if (strName.compare("每天提醒")==0)
	{
		return TT_DAILY;
	}
	if (strName.compare("每周提醒")==0)
	{
		return TT_WEEKLY;
	}
	if (strName.compare("每月提醒")==0)
	{
		return TT_MONTHLY;
	}
	return (TaskType)-1;
}

void ITask::GetTaskTypes( vector<string> &vecTypes )
{
	vecTypes.clear();
	vecTypes.push_back("一次提醒");
	vecTypes.push_back("每天提醒");
	vecTypes.push_back("每周提醒");
	vecTypes.push_back("每月提醒");
}

bool ITask::operator == (const ITask &task)
{
	return  Id==task.Id
		&& Type==task.Type
		&& TaskTime == task.TaskTime
		&& LastRunTime == task.LastRunTime
		&& CreateTime == task.CreateTime
		&& Tip == task.Tip;	//提示内容
}

int ITask::GetMonthWeek(const CTime &tm)
{
	return tm.GetYear() - CTime(0).GetYear();
}

ATL::CString ITask::GetWeekDayName( int iDay )
{
	static UINT DayOfWeek[] = {
		LOCALE_SDAYNAME7,   // Sunday
		LOCALE_SDAYNAME1,   
		LOCALE_SDAYNAME2,
		LOCALE_SDAYNAME3,
		LOCALE_SDAYNAME4, 
		LOCALE_SDAYNAME5, 
		LOCALE_SDAYNAME6,   // Saturday
	};
	TCHAR strWeekday[256];

	::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
		DayOfWeek[iDay-1],   // Get day of week from CTime
		strWeekday, sizeof(strWeekday));
	return strWeekday;
}