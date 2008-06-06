#include ".\taskdb.h"
#include <ATLComTime.h>
#include "SQLite/CppSQLite3.h"

TaskDB g_TaskDB;

//////////////////////////////////////////////////////////////////////////
//时间与字符串互相转换函数
ATL::CString TimeToString(CTime t)
{
	ATL::CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

CTime StringToTime(ATL::CString strTime)
{;  
	COleDateTime   tm;  
	tm.ParseDateTime(strTime);  
	SYSTEMTIME   st;  
	tm.GetAsSystemTime(st);  
	return CTime(st);   
}
//////////////////////////////////////////////////////////////////////////


bool WriteTaskToDB(CppSQLite3DB &dbTask, const ITask &task)
{
	ATL::CString strTime = TimeToString(CTime::GetCurrentTime());
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
	//	"type integer, "			//其中的type字段由ITask::TaskType定义。
	//	"time char[32], "			//任务时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
	//	"last_run_time char[32], "	//最后执行时间
	//	"task_create_time char[32], "	//任务创建时间
	//	"tip char[2048] );"			//提示语句
	strSql.format("insert into T_task values(NULL,%d, '%q','%q','%q','%q');",
		task.Type,
		TimeToString(task.TaskTime),
		TimeToString(task.LastRunTime),
		TimeToString(task.CreateTime),
		task.Tip
		);
	try{
		dbTask.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
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
		//10-55 = 有些事情，是可以留到明天再做的
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
		AddDailyTask(10,55,"有些事情，是可以留到明天再做的");
		AddDailyTask(21,55,"总结一下今天的工作，写一下日记，准备休息啦！");
		AddDailyTask(22,55,"夜了，拉灯，睡觉！");
		AddDailyTask(23,55,"注意身体，多休息。");
		//////////////////////////////////////////////////////////////////////////
		return true;
	}
	CppSQLite3Query q = dbTask.execQuery("select id, type, time, last_run_time, task_create_time, tip from T_task");
	while (!q.eof())
	{
		ITask task;
		task.Id = q.getIntField("id");
		task.Type = q.getIntField("type");
		task.TaskTime = StringToTime(q.getStringField("time"));
		task.LastRunTime = StringToTime(q.getStringField("last_run_time"));
		task.CreateTime = StringToTime(q.getStringField("task_create_time"));
		task.Tip = q.getStringField("tip");

		m_vecTask.push_back(task);

		q.nextRow();
	}
	return true;
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

		//为类型字段建立索引
		dbTask.execDML("create index idx_type on T_task(type);");
	}
	else
	{
		//清空旧数据
		dbTask.execDML("delete from T_task;");
	}

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		WriteTaskToDB(dbTask,*it);
	}

	dbTask.execDML("end;");                                    //结束保存事务，提交数据
	return true;
}

int TaskDB::AddTask(const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	int id = 0;
	if (!m_vecTask.empty())
	{
		id = m_vecTask.back().Id + 1;
	}
	m_vecTask.push_back(task);
	m_vecTask.back().Id = id;
	return -1;
}

bool TaskDB::RemoveTask( int idTask )
{
	LOCK_THIS_RANGE(m_cs);

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
	task.LastRunTime = task.CreateTime;
	task.Tip = strMessage;
	AddTask(task);
	return false;
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