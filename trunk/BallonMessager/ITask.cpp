#include ".\itask.h"

ITask::ITask(void)
{
}

ITask::~ITask(void)
{
}

bool ITask::IsTimeNow()
{
	switch(Type)
	{
	case TT_DAILY:
		{
			CTime tmNow = CTime::GetCurrentTime();
			

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
	}
	return false;
}