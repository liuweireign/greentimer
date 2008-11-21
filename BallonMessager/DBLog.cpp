#include "stdafx.h"
#include ".\dblog.h"
#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "Globe.h"

DBLog g_DBLog;

bool CheckCreateLogTable(CppSQLite3DB &dbTask)
{
	try{
		//如果表格不存在，则创建表格
		if (!dbTask.tableExists("T_log"))                //创建事件日志表
		{
			//数据库字段：任务id，任务类型，任务时间，上次提示时间，提示语句等。
			//last_run_time可以用来辅助确定提示是否已经执行，避免重复
			dbTask.execDML("Create table T_log("
				"id integer PRIMARY KEY AUTOINCREMENT, "	//唯一id
				"modal char[128],"
				"time_log integer, "		//时间
				"type integer, "		//状态
				"value1 integer, "		//值1
				"value2 integer, "		//值2
				"message char[1024]);"	//提示语句
				);

			//为类型字段建立索引
			dbTask.execDML("create index idx_time on T_log(time_log);");
		}
		return true;
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

DBLog::DBLog(void)
{
	m_strDB = Globe::GetDBPath();
}

DBLog::~DBLog(void)
{
}

void DBLog::Log( const char *pModel,int code,int value1,int value2, const char *pMessage )
{
	//打开数据库
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateLogTable(dbTask);

	CppSQLite3Buffer strSql;
	strSql.format("insert into T_Log values(NULL,'%q',%d,%d,%d,%d,'%q');",
		pModel,GlobeFuns::TimeToInt(CTime::GetCurrentTime()),code,value1,value2,pMessage
		);

	try{
		if(1!=dbTask.execDML(strSql))
		{
			ATLASSERT(FALSE);
			return;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return;
	}
}


const char *LOG_CONST::MODAL_MAIN = "MAIN";
const char *LOG_CONST::MODAL_NOTIFY = "NOTIFY";
const char *LOG_CONST::MODAL_TODO = "TODO";
