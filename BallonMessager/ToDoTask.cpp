#include "StdAfx.h"
#include ".\todotask.h"

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "DBLog.h"

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
//
//
//bool WriteToDoToDB(CppSQLite3DB &dbTodo, const ToDoTask &todo)
//{
//	CppSQLite3Buffer strSql;
//
//	//"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
//	//	"title char[128], "			//标题
//	//	"priority integer, "		//优先级
//	//	"state integer, "			//状态
//	//	"time_create char[32], "	//任务创建时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
//	//	"time_start char[32], "		//开始时间
//	//	"time_finish char[32], "	//结束时间
//	//	"remark char[64*1024]);"	//提示语句
//	strSql.format("insert into T_todo2 values(NULL,'%q',%d,%d,'%q','%q','%q','%q');",
//		//todo.id,
//		todo.strTask.c_str(),
//		todo.priority,
//		todo.state,
//		GlobeFuns::TimeToString(todo.tmCreateTime),
//		GlobeFuns::TimeToString(todo.tmStartWorkTime),
//		GlobeFuns::TimeToString(todo.tmPlanFinshTime),
//		todo.strRemark.c_str()
//		);
//	try{
//		dbTodo.execDML(strSql);
//	}
//	catch(CppSQLite3Exception &exp)
//	{
//		exp;
//		ATLTRACE("error:%s\n",exp.errorMessage());
//		ATLASSERT(FALSE);
//		return false;
//	}
//	return true;
//}
bool UpdateToDB(CppSQLite3DB &dbTodo, const ToDoTask &todo)
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
	strSql.format("update T_todo2 set"
		" title='%q', priority=%d, state=%d,"
		" time_create=%d,time_start=%d,time_finish=%d,remark='%q'"
		" where id=%d;",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToInt(todo.tmCreateTime),
		GlobeFuns::TimeToInt(todo.tmStartWorkTime),
		GlobeFuns::TimeToInt(todo.tmPlanFinshTime),
		todo.strRemark.c_str(),
		todo.id
		);
	try{
		return 1==dbTodo.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

bool CheckCreateTable(CppSQLite3DB &dbTask)
{
	try{
		//如果表格不存在，则创建表格
		if (!dbTask.tableExists("T_todo2"))                //创建事件日志表
		{
			//数据库字段：任务id，任务类型，任务时间，上次提示时间，提示语句等。
			//last_run_time可以用来辅助确定提示是否已经执行，避免重复
			dbTask.execDML("Create table T_todo2("
				"id integer PRIMARY KEY AUTOINCREMENT, "	//任务唯一id
				"title char[128], "			//标题
				"priority integer, "		//优先级
				"state integer, "			//状态
				"time_create integer, "	//任务创建时间。时间以"年-月-日 时:分:秒"这样的格式储存，下同
				"time_start integer, "		//开始时间
				"time_finish integer, "	//结束时间
				"remark char[64*1024]);"	//提示语句
				);

			//为类型字段建立索引
			dbTask.execDML("create index idx_todo2_time_create on T_todo2(time_create);");

			LOG_TODO(LOG_CONST::MTV_CREATE_TABLE,-1,"成功:创建表 ");
		}
		return true;
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_CREATE_TABLE,-1,((string)"异常:创建表 " + exp.errorMessage()).c_str());
		return false;
	}
}

TodoSet::TodoSet()
{
	m_strDB = GlobeFuns::GetAppDirectory() + _T("task.db");
}

void TodoSet::GetTodoList( std::set<int> &taskIDs )
{
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_todo2"))                //创建事件日志表
	{
		ATLASSERT(FALSE);
		return;
	}

	CppSQLite3Buffer buf;
	buf.format("select id from T_todo2 where state!=%d;",(int)ToDoTask::TS_DELETED);
	CppSQLite3Query q = dbTask.execQuery(buf);
	while(!q.eof())
	{
		ATLASSERT(ToDoTask::ERROR_TASKID!=q.getIntField("id"));
		taskIDs.insert(q.getIntField("id"));
		q.nextRow();
	}
}

ToDoTask TodoSet::GetToDo( int id )
{
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_todo2"))                //创建事件日志表
	{
		return ToDoTask(ToDoTask::ERROR_TASKID);
	}

	CppSQLite3Buffer buf;
	buf.format("select id, title,priority , state, time_create, time_start, time_finish, remark  from T_todo2 where id=%d;",id);
	CppSQLite3Query q = dbTask.execQuery(buf);
	if(!q.eof())
	{
		ToDoTask todo;

		todo.id = q.getIntField("id");
		todo.strTask = q.getStringField("title");
		todo.priority = (ToDoTask::TaskPriority)q.getIntField("priority");
		todo.state = (ToDoTask::TaskState)q.getIntField("state");
		todo.tmCreateTime = GlobeFuns::IntToTime(q.getIntField("time_create"));
		todo.tmStartWorkTime = GlobeFuns::IntToTime(q.getIntField("time_start"));
		todo.tmPlanFinshTime = GlobeFuns::IntToTime(q.getIntField("time_finish"));
		todo.strRemark = q.getStringField("remark");

		return todo;
		//q.nextRow();
	}
	return ToDoTask(ToDoTask::ERROR_TASKID);
}

bool TodoSet::UpdateToDo( const ToDoTask &task )
{
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	return UpdateToDB(dbTask,task);
}

int TodoSet::AddToDo()
{
	ToDoTask todo(0);
	todo.tmCreateTime = CTime::GetCurrentTime();
	todo.strTask = _T("新任务");
	todo.priority = ToDoTask::TP_NORMAL;
	todo.state = ToDoTask::TS_NOT_START;
	todo.tmPlanFinshTime = 0;
	todo.tmStartWorkTime = 0;

	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	
	CppSQLite3Buffer strSql;
	strSql.format("insert into T_todo2 values(NULL,'%q',%d,%d,%d,%d,%d,'%q');",
		todo.strTask.c_str(),
		todo.priority,
		todo.state,
		GlobeFuns::TimeToInt(todo.tmCreateTime),
		GlobeFuns::TimeToInt(todo.tmStartWorkTime),
		GlobeFuns::TimeToInt(todo.tmPlanFinshTime),
		todo.strRemark.c_str()
		);
	try{
		if(1!=dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_ADD,-1,"失败:添加待办事项失败。");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_ADD,-1,((string)"异常:" + exp.errorMessage()).c_str());
		return false;
	}

	LOG_TODO(LOG_CONST::MTV_ADD,(int)dbTask.lastRowId(),"成功:添加待办事项成功。");
	return (int)dbTask.lastRowId();
}

bool TodoSet::DeleteToDo( int id )
{
	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	strSql.format("delete from T_todo2 where id=%d;",id);
	try{
		if(1==dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_DEL,id,"成功:删除待办事项成功");
			return true;
		}
		else
		{
			LOG_TODO(LOG_CONST::MTV_DEL,id,"失败:删除待办事项失败");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_DEL,id,((string)"异常:删除待办事项失败" + exp.errorMessage()).c_str());
		return false;
	}
}

bool TodoSet::CheckDBValid()
{
	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	return dbTask.tableExists("T_todo2");
}