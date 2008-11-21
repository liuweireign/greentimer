#include "StdAfx.h"
#include ".\todotask.h"

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "DBLog.h"
#include "globe.h"

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

TCHAR * ToDoTask::PriorityText( TaskPriority priority )
{
	switch(priority)
	{
	case TP_MOST_IMPORTANT: return _T("紧急");
	case TP_IMPORTANT: return _T( "重要");
	case TP_NORMAL: return _T("普通");
	case TP_NOT_IMPORTANT: return _T("不重要"); 
	}
	return _T("错误！");
}

TCHAR * ToDoTask::StateText( TaskState state )
{
	switch(state)
	{
	case TS_NOT_START: return _T("计划中");
	case TS_WORKING: return _T( "工作中");
	case TS_STOP: return _T("暂停");
	case TS_FINISHED: return _T("已完成"); 
	case TS_CANCEL: return _T( "取消");
	case TS_DELETED: return _T("删除");
	}
	return _T("错误！");
}

COLORREF ToDoTask::GetStateColor( TaskState state )
{
	switch(state)
	{
	case ToDoTask::TS_NOT_START:
		return RGB(0XCC,0XFF,0XCC);
	case ToDoTask::TS_WORKING:
		return RGB(10,170,10);
	case ToDoTask::TS_STOP:
		return RGB(10,100,10);
	case ToDoTask::TS_CANCEL:
		return RGB(0XCC,0XCC,0XCC);
	case ToDoTask::TS_FINISHED:
		return RGB(0XAA,0XAA,0XAA);
	case TS_DELETED: 
		return RGB(0X55,0X55,0X55);

	}
	return RGB(0,0,0);
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
	m_strDB = Globe::GetDBPath();

	//RecoverDataFromV1();
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
	if (task.state==ToDoTask::TS_FINISHED)
	{
		//如果旧的任务状态不是“已完成”，那么先把任务状态设为完成。
		ToDoTask todo = GetToDo(task.id);
		if (ToDoTask::TS_FINISHED!=todo.state)
		{
			UpdateTaskFinishTime(task.id,CTime::GetCurrentTime());
		}
	}

	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);
	return UpdateToDB(dbTask,task);
}

void TodoSet::UpdateTaskFinishTime( int id,const CTime &tm)
{
	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	//strSql.format("delete from T_todo2 where id=%d;",id);
	strSql.format("update T_todo2 set time_finish=%d where id=%d;",GlobeFuns::TimeToInt(tm),id);
	try{
		if(1==dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_SETFINISH,id,"成功:设置最后更新时间");
			return ;
		}
		else
		{
			LOG_TODO(LOG_CONST::MTV_SETFINISH,id,"失败:设置最后更新时间");
			return ;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_SETFINISH,id,((string)"异常:设置最后更新时间" + exp.errorMessage()).c_str());
		return ;
	}
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
	//如果任务状态不是“已完成”，那么先把任务状态设为完成。
	ToDoTask todo = GetToDo(id);
	if (ToDoTask::TS_FINISHED!=todo.state)
	{
		UpdateTaskFinishTime(id,CTime::GetCurrentTime());
	}

	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	//strSql.format("delete from T_todo2 where id=%d;",id);
	strSql.format("update T_todo2 set state=%d where id=%d;",ToDoTask::TS_DELETED,id);
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

bool TodoSet::ReactiveToDo( int id )
{
	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	CppSQLite3Buffer strSql;
	//strSql.format("delete from T_todo2 where id=%d;",id);
	strSql.format("update T_todo2 set state=%d where id=%d;",ToDoTask::TS_NOT_START,id);
	try{
		if(1==dbTask.execDML(strSql))
		{
			LOG_TODO(LOG_CONST::MTV_REACTIVE,id,"成功:重新激活待办事项成功");
			return true;
		}
		else
		{
			LOG_TODO(LOG_CONST::MTV_REACTIVE,id,"失败:重新激活待办事项失败");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_REACTIVE,id,((string)"异常:重新激活待办事项失败" + exp.errorMessage()).c_str());
		return false;
	}
}


bool TodoSet::DeleteToDoForever( int id )
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
			LOG_TODO(LOG_CONST::MTV_DELETEFOREVER,id,"成功:永久删除待办事项成功");
			return true;
		}
		else
		{
			LOG_TODO(LOG_CONST::MTV_DELETEFOREVER,id,"失败:永久删除待办事项失败");
			return false;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		LOG_TODO(LOG_CONST::MTV_DELETEFOREVER,id,((string)"异常:永久删除待办事项失败" + exp.errorMessage()).c_str());
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

bool TodoSet::RecoverDataFromV1()
{
	vector<ToDoTask> vecOldToDos;
	{
		//打开数据库
		CppSQLite3DB dbTask;
		dbTask.open(m_strDB.c_str());
		CheckCreateTable(dbTask);

		//insert into t_todo2(id,title,priority,state,time_create,time_start,time_finish,remark)
		// select NULL,title,priority,state,1219975645,0,0,remark from t_todo;

		//如果表格不存在，则说明这数据库格式不对
		if (!dbTask.tableExists("T_todo"))                //创建事件日志表
		{
			return true;
		}

		CppSQLite3Buffer buf;
		buf.format("select id, title,priority , state, time_create, time_start, time_finish, remark  from T_todo");
		CppSQLite3Query q = dbTask.execQuery(buf);
		while(!q.eof())
		{
			ToDoTask todo;

			todo.id = 0;//q.getIntField("id");
			todo.strTask = q.getStringField("title");
			todo.priority = (ToDoTask::TaskPriority)q.getIntField("priority");
			todo.state = (ToDoTask::TaskState)q.getIntField("state");
			todo.tmCreateTime = GlobeFuns::StringToTime(q.getStringField("time_create"));
			todo.tmStartWorkTime = GlobeFuns::StringToTime(q.getStringField("time_start"));
			todo.tmPlanFinshTime = GlobeFuns::StringToTime(q.getStringField("time_finish"));
			todo.strRemark = q.getStringField("remark");

			vecOldToDos.push_back(todo);
			q.nextRow();
		}
	}

	bool bSucceed = true;
	for(int i=0;i<(int)vecOldToDos.size();i++)
	{
		ToDoTask todo = vecOldToDos[i];
		todo.id = AddToDo();
		if(!UpdateToDo(todo))
		{
			bSucceed = false;
		}
	}
	if (bSucceed)
	{
		//打开数据库
		CppSQLite3DB dbTask;
		dbTask.open(m_strDB.c_str());
		dbTask.execDML("drop table t_todo;");
	}

	return true;
}

void TodoSet::GetHistoryTodoList( std::set<int> &taskIDs )
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
	buf.format("select id from T_todo2 where state=%d;",(int)ToDoTask::TS_DELETED);
	CppSQLite3Query q = dbTask.execQuery(buf);
	while(!q.eof())
	{
		ATLASSERT(ToDoTask::ERROR_TASKID!=q.getIntField("id"));
		taskIDs.insert(q.getIntField("id"));
		q.nextRow();
	}
}

bool TodoSet::GetStatic( int &iFinished,int &iPlaned, int &iWorking )
{
	iFinished = GetTaskCount(ToDoTask::TS_FINISHED);
	iPlaned = GetTaskCount(ToDoTask::TS_NOT_START) + GetTaskCount(ToDoTask::TS_STOP);
	iWorking = GetTaskCount(ToDoTask::TS_WORKING);
	return iFinished>=0 
		&& iPlaned>=0 
		&& iWorking>=0;
}

int TodoSet::GetTaskCount( int iState )
{
	//打开数据
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateTable(dbTask);

	//如果表格不存在，则说明这数据库格式不对
	if (!dbTask.tableExists("T_todo2"))                //创建事件日志表
	{
		ATLASSERT(FALSE);
		return -1;
	}

	CppSQLite3Buffer buf;
	if (iState>=0)
	{
		buf.format("select count(*) as count from T_todo2 where state=%d;",iState);
	}
	else
	{
		buf.format("select count(*) as count from T_todo2;");
	}
	CppSQLite3Query q = dbTask.execQuery(buf);
	if(!q.eof())
	{
		return q.getIntField("count");
	}
	return -1;
}