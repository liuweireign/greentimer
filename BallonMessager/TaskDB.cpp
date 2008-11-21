#include "stdafx.h"
#include ".\taskdb.h"
#include <ATLComTime.h>
#include <algorithm>

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "Globe.h"
#include "DBLog.h"

TaskDB g_TaskDB;


bool WriteTaskToDB(CppSQLite3DB &dbTask, const ITask &task)
{
	ATL::CString strTime = GlobeFuns::TimeToString(CTime::GetCurrentTime());
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
	//	"type integer, "			//其中的type字段由ITask::TaskType定义。
	//	"time char[32], "			//任务时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
	//	"last_run_time char[32], "	//最后执行时间
	//	"task_create_time char[32], "	//任务创建时间
	//	"tip char[2048] );"			//提示语句
	strSql.format("insert into T_task values(NULL,%d, '%q','%q','%q','%q');",
		task.Type,
		GlobeFuns::TimeToString(task.TaskTime),
		GlobeFuns::TimeToString(task.LastRunTime),
		GlobeFuns::TimeToString(task.CreateTime),
		task.Tip
		);
	try{
		dbTask.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

TaskDB::TaskDB(void)
{
}

TaskDB::~TaskDB(void)
{
}

bool CompTaskID(const ITask &taskLeft,const ITask &taskRight)
{
	return taskLeft.Id<taskRight.Id;
}
bool TaskDB::ReadFromDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);

	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_task"))                //创建事件日志表
	{
		//////////////////////////////////////////////////////////////////////////
		//增加默认的初始任务
		//0-55,"注意身体，多休息。
		//1-55,"注意身体，多休息。
		//2-55  = 注意身体，多休息。
		//3-55  = 注意身体，多休息。
		//4-55  = 注意身体，多休息。
		//5-55  = 早上好，多锻炼。
		//6-55  = 早上好。
		//7-55  = 早上好。
		//8-55  = 上班了，先定个工作计划
		//9-55  = 中场休息一会儿吧。
		//10-55 = 工作两个小时了，该做些运动啦!
		//11-55 = 下班了，好好休息。
		//12-55 = 中午休息一下，身体是革命的本钱
		//13-55 = 定个计划，下午做什么？
		//14-55 = 起来喝口水，伸伸懒腰，放松一下
		//15-55 = 休息一下。
		//16-55 = 休息一下。
		//17-55 = 下班时间到。整理一下手头的工作，用愉快的心情回家吧。
		//18-55 = 在工作还是在娱乐？保重身体哦。
		//19-55 = 在工作还是在娱乐？如果在家就出去散散步吧。
		//20-55 = 有些事情，是可以留到明天再做的
		//21-55 = 总结一下今天的工作，写一下日记，准备休息啦！
		//22-55 = 夜了，拉灯，睡觉！
		//23-55 = 注意身体，多休息。
		AddDailyTask(0,55,"注意身体，多休息。");
		AddDailyTask(1,55,"注意身体，多休息。");
		AddDailyTask(2,55,"注意身体，多休息。");
		AddDailyTask(3,55,"注意身体，多休息。");
		AddDailyTask(4,55,"注意身体，多休息。");
		AddDailyTask(5,55,"早上好，多锻炼。");
		AddDailyTask(6,55,"早上好，多锻炼。");
		AddDailyTask(7,55,"早上好，多锻炼。");
		AddDailyTask(8,55,"上班了，先定个工作计划");
		AddDailyTask(9,55,"中场休息一会儿吧。");
		AddDailyTask(10,55,"工作两个小时了，该做些运动啦!");
		AddDailyTask(11,55,"下班了，好好休息。");
		AddDailyTask(12,55,"中午休息一下，身体是革命的本钱");
		AddDailyTask(13,55,"定个计划，下午做什么？");
		AddDailyTask(14,55,"起来喝口水，伸伸懒腰，放松一下");
		AddDailyTask(15,55,"休息一下。");
		AddDailyTask(16,55,"休息一下。");
		AddDailyTask(17,55,"下班时间到。整理一下手头的工作，用愉快的心情回家吧。");
		AddDailyTask(18,55,"在工作还是在娱乐？保重身体哦。");
		AddDailyTask(19,55,"在工作还是在娱乐？如果在家就出去散散步吧。");
		AddDailyTask(20,55,"有些事情，是可以留到明天再做的");
		AddDailyTask(21,55,"总结一下今天的工作，写一下日记，准备休息啦！");
		AddDailyTask(22,55,"夜了，拉灯，睡觉！");
		AddDailyTask(23,55,"注意身体，多休息。");

		CTime tmTime = CTime::GetCurrentTime();
		tmTime += CTimeSpan(100,0,0,0);
		AddTimeTask(tmTime,"您使用“绿色时间管理”已经100天了，感谢您对我们的支持！"
			"现在可能有新版本啦，到 http://code.google.com/p/greentimer/ 看看？");
		//////////////////////////////////////////////////////////////////////////
		return true;
	}
	CppSQLite3Query q = dbTask.execQuery("select id, type, time, last_run_time, task_create_time, tip from T_task");
	while (!q.eof())
	{
		ITask task;
		task.Id = q.getIntField("id");
		task.Type = (ITask::TaskType)q.getIntField("type");
		task.TaskTime = GlobeFuns::StringToTime(q.getStringField("time"));
		task.LastRunTime = GlobeFuns::StringToTime(q.getStringField("last_run_time"));
		task.CreateTime = GlobeFuns::StringToTime(q.getStringField("task_create_time"));
		task.Tip = q.getStringField("tip");

		m_vecTask.push_back(task);

		q.nextRow();
	}
	//数据必须按ID排好顺序，否则后面添加新任务的时候，ID号可能不正确。
	sort(m_vecTask.begin(),m_vecTask.end(),CompTaskID);
	return true;
}

bool TaskDB::ReadFromDB()
{
	return g_TaskDB.ReadFromDB(Globe::GetDBPath().c_str());
}
bool TaskDB::SaveToDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	dbTask.execDML("begin;");                                 //开始保存事务

	//如果表格不存在，则创建表格
	if (!dbTask.tableExists("T_task"))                //创建事件日志表
	{
		//数据库字段：任务id，任务类型，任务时间，上次提示时间，提示语句等。
		//last_run_time可以用来辅助确定提示是否已经执行，避免重复
		dbTask.execDML("Create table T_task("
			"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
			"type integer, "			//其中的type字段由ITask::TaskType定义。
			"time char[32], "			//任务时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
			"last_run_time char[32], "	//最后执行时间
			"task_create_time char[32], "	//任务创建时间
			"tip char[2048] );"			//提示语句
			);

		////为类型字段建立索引
		//dbTask.execDML("create index idx_type on T_task(type);");
		//LOG_NOTIFY(LOG_CONST::MNV_CREATE_TABLE,-1,"创建提醒表");
	}
	else
	{
		//清空旧数据
		dbTask.execDML("delete from T_task;");
	}

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(!WriteTaskToDB(dbTask,*it))
		{
			dbTask.execDML("rollback transaction;");
			return false;
		}
	}

	dbTask.execDML("end;");                                    //结束保存事务，提交数据
	return true;
}

bool TaskDB::SaveToDB()
{
	return g_TaskDB.SaveToDB(Globe::GetDBPath().c_str());
}
bool TaskDB::AddTask(const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	int id = 0;
	if (!m_vecTask.empty())
	{
		id = m_vecTask.back().Id + 1;
	}
	m_vecTask.push_back(task);
	m_vecTask.back().Id = id;

	LOG_NOTIFY(LOG_CONST::MNV_ADD,id,"成功:增加定时提醒");
	return true;
}

bool TaskDB::RemoveTask( int idTask )
{
	LOCK_THIS_RANGE(m_cs);
	LOG_NOTIFY(LOG_CONST::MNV_DEL,idTask,"删除定时提醒");

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			m_vecTask.erase(it);
			return true;
		}
	}
	return false;
}

void TaskDB::GetTaskList( std::vector<int> &vecIdOut )
{
	LOCK_THIS_RANGE(m_cs);

	vecIdOut.clear();
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		vecIdOut.push_back((*it).Id);
	}
}

bool TaskDB::GetTask(int idTask,ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::FindTaskRunNow(ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if ((*it).IsTimeNow())
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::AddDailyTask( int iHour, int iMinute, ATL::CString strMessage )
{
	ITask task;
	task.Type = ITask::TT_DAILY;
	task.CreateTime = CTime::GetCurrentTime();
	task.TaskTime = CTime(2000,1,1,iHour,iMinute,0);
	task.LastRunTime = 0;
	task.Tip = strMessage;
	return AddTask(task);
}

bool TaskDB::AddTimeTask( const CTime &tmRuntime, ATL::CString strMessage )
{
	ITask task;
	task.Type = ITask::TT_ONCE;
	task.CreateTime = CTime::GetCurrentTime();
	task.TaskTime = tmRuntime;
	task.LastRunTime = 0;
	task.Tip = strMessage;
	return AddTask(task);
}
bool TaskDB::UpdateTask( const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(task.Id==(*it).Id)
		{
			*it = task;
			return true;
		}
	}
	return false;
}
// 按任务类型及执行时间查询任务
bool TaskDB::GetTask(CTime taskTime, int taskType, ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(taskTime == (*it).TaskTime &&
			taskType == (*it).Type
			)
		{
			task = *it;
			return true;
		}
	}
	return false;
}

// 通过时间和类型查询任务,时、分用字符串表示
bool TaskDB::GetTask(ATL::CString strHour, ATL::CString strMin, int taskType, ITask & task)
{
	int iHour = atoi(strHour);
	int iMin = atoi(strMin);

	return GetTask(iHour, iMin, taskType, task);
}

// 通过时间查询任务，时、分用int方式表示
bool TaskDB::GetTask(int iHour, int iMin, int taskType, ITask & task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		ITask theTask;
		theTask = *it;

		CTime taskTime = theTask.TaskTime;

		if(iHour == taskTime.GetHour() &&
			iMin == taskTime.GetMinute() &&
			taskType == theTask.Type
			)
		{
			task = *it;
			return true;
		}
	}
	return false;
}
