#include "StdAfx.h"
#include ".\todotask.h"

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"

using namespace std;

TodoSet g_todoSet;

ToDoTask::ToDoTask(void)
{
	id = ERROR_TASKID;
}

ToDoTask::ToDoTask(int taskid)
{
	this->id = taskid;
}
ToDoTask::~ToDoTask(void)
{
}

bool ToDoTask::operator < (const ToDoTask &task) const
{
	return id<task.id;
}


bool WriteToDoToDB(CppSQLite3DB &dbTodo, const ToDoTask &todo)
{
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
	//	"title char[128], "			//标题
	//	"priority integer, "		//优先级
	//	"state integer, "			//状态
	//	"time_create char[32], "	//任务创建时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
	//	"time_start char[32], "		//开始时间
	//	"time_finish char[32], "	//结束时间
	//	"remark char[64*1024]);"	//提示语句
	strSql.format("insert into T_todo values(NULL,'%q',%d,%d,'%q','%q','%q','%q');",
		//todo.id,
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		TimeToString(todo.tmCreateTime),
		TimeToString(todo.tmStartWorkTime),
		TimeToString(todo.tmPlanFinshTime),
		todo.strRemark.c_str()
		);
	try{
		dbTodo.execDML(strSql);
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

TodoSet::TodoSet()
{
}

bool TodoSet::Load(const TCHAR *strDB)
{

	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_todo"))                //创建事件日志表
	{
		return false;
	}

	m_setTask.clear();

	CppSQLite3Query q = dbTask.execQuery("select id, title,priority , state, time_create, time_start, time_finish, remark  from T_todo");
	while (!q.eof())
	{
		ToDoTask todo;
		
		todo.id = q.getIntField("id");
		todo.strTask = q.getStringField("title");
		todo.priority = (ToDoTask::TaskPriority)q.getIntField("priority");
		todo.state = (ToDoTask::TaskState)q.getIntField("state");
		todo.tmCreateTime = StringToTime(q.getStringField("time_create"));
		todo.tmStartWorkTime = StringToTime(q.getStringField("time_start"));
		todo.tmPlanFinshTime = StringToTime(q.getStringField("time_finish"));
		todo.strRemark = q.getStringField("remark");

		bool bSucceed = m_setTask.insert(todo).second;
		ATLASSERT(bSucceed);
		q.nextRow();
	}
	return true;
}

bool TodoSet::Load()
{
	return Load(GlobeFuns::GetAppDirectory() + "task.db");
}
bool TodoSet::Save(const TCHAR *strDB)
{
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	dbTask.execDML("begin;");                                 //开始保存事务

	//如果表格不存在，则创建表格
	if (!dbTask.tableExists("T_todo"))                //创建事件日志表
	{
		//数据库字段：任务id，任务类型，任务时间，上次提示时间，提示语句等。
		//last_run_time可以用来辅助确定提示是否已经执行，避免重复
		dbTask.execDML("Create table T_todo("
			"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
			"title char[128], "			//标题
			"priority integer, "		//优先级
			"state integer, "			//状态
			"time_create char[32], "	//任务创建时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
			"time_start char[32], "		//开始时间
			"time_finish char[32], "	//结束时间
			"remark char[64*1024]);"	//提示语句
			);

		//为类型字段建立索引
		dbTask.execDML("create index idx_time_create on T_todo(time_create);");
	}
	else
	{
		//清空旧数据
		dbTask.execDML("delete from T_todo;");
	}

	std::set<ToDoTask>::iterator it = m_setTask.begin();
	for (;it!=m_setTask.end();it++)
	{
		if(!WriteToDoToDB(dbTask,*it))
		{
			dbTask.execDML("rollback transaction;");
			return false;
		}
	}

	dbTask.execDML("end;");                                    //结束保存事务，提交数据
	return true;
}

bool TodoSet::Save()
{
	return Save(GlobeFuns::GetAppDirectory() + "task.db");
}

void TodoSet::GetTodoList( std::set<int> &taskIDs )
{
	set<ToDoTask>::const_iterator it =  m_setTask.begin();
	for (;it!=m_setTask.end();it++)
	{
		taskIDs.insert((*it).id);
	}
}

ToDoTask TodoSet::GetToDo( int id )
{
	set<ToDoTask>::const_iterator it = m_setTask.find(id);
	if (it!=m_setTask.end())
	{
		return *it;
	}
	else
	{
		return ToDoTask(ToDoTask::ERROR_TASKID);
	}
}

bool TodoSet::UpdateToDo( const ToDoTask &task )
{
	if(ToDoTask::ERROR_TASKID==task.id)
	{
		ATLASSERT(FALSE);
		return false;
	}

	if(!DeleteToDo(task.id))
	{
		return false;
	}
	return m_setTask.insert(task).second;
}

int TodoSet::AddToDo()
{
	int new_id = -1;
	if (m_setTask.empty())
	{
		new_id = 1;
	}
	else
	{
		new_id = (*m_setTask.end()).id + 1;
	}
	if(!m_setTask.insert(ToDoTask(new_id)).second)
	{
		return ToDoTask::ERROR_TASKID;
	}
	return new_id;
}

bool TodoSet::DeleteToDo( int id )
{
	return 1==m_setTask.erase(ToDoTask(id));
}