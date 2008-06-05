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
		CTime tmNow = CTime::GetCurrentTime();
		if (tmNow.GetHour()==TaskTime.GetHour() && tmNow.GetMinute()==TaskTime.GetMinute())
		{
			return true;
		}
		break;
	}
	return false;
}