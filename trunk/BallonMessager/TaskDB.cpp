#include ".\taskdb.h"
#include <ATLComTime.h>
#include "SQLite/CppSQLite3.h"

//////////////////////////////////////////////////////////////////////////
//时间与字符串互相转换函数
CString TimeToString(CTime t)
{
	CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

CTime StringToTime(CString strTime)
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
	CString strTime = TimeToString(CTime::GetCurrentTime());
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
	//	"type integer, "			//其中的type字段由ITask::TaskType定义。
	//	"time char[32], "			//任务时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
	//	"last_run_time char[32], "	//最后执行时间
	//	"task_create_time char[32], "	//任务创建时间
	//	"tip char[2048] );"			//提示语句
	strSql.format("insert into T_task values(NULL,%d, '%q','%q','%q');",
		task.Type,
		TimeToString(task.TaskTime),
		TimeToString(task.LastRunTime),
		TimeToString(task.CreateTime),
		task.Tip
		);
	try{
		dbTask.execDML("begin");                                 //开始保存事务
		dbTask.execDML(strSql);
		dbTask.execDML("end");                                    //结束保存事务，提交数据
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
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_task"))                //创建事件日志表
	{
		return false;
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
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

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

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		WriteTaskToDB(dbTask,*it);
	}
	return true;
}

int TaskDB::AddTask(const ITask &task )
{
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
	vecIdOut.clear();
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		vecIdOut.push_back((*it).Id);
	}
}

ITask * TaskDB::GetTask( int idTask )
{
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			return &(*it);
		}
	}
	return NULL;
}

ITask * TaskDB::TaskRunNow()
{
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		ITask *pTask = &(*it);
		if (pTask->IsTimeNow())
		{
			return pTask;
		}
	}
	return NULL;
}