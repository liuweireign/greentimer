#pragma once
#include <string>

/**
* 日志记录器.
*
* 这个日志将会写入到task.db中去
* 
*
*/
class DBLog
{
public:
	DBLog(void);
	~DBLog(void);

	/// 记录日志.
	/// 
	/// 参数分别是：模块名、代号、自定义值1、自定义值2、日志内容。
	/// 自定义值是为了方便程序解析和统计
	void Log(const char *pModel,int code,int value1,int value2, const char *pMessage);
private:
	std::string m_strDB;
};

extern DBLog g_DBLog;
 
/*

三类日志：
一、开机关机；
模块名为"MAIN",code = 0，开机value1=0,关机value1=1;

二、提醒：
增加、删除、修改、显示提醒
模块名为"NOTIFY",code = 1，value1的值按增加、删除、修改、显示分别为0，1，2，3

三、任务：
增加、删除、修改任务
模块名为"TODO",code = 2，value1的值按增加、删除、修改分别为0，1，2
*/
class LOG_CONST
{
public:
	static const char *MODAL_MAIN ;
	static const char *MODAL_NOTIFY;
	static const char *MODAL_TODO;

	enum MODEL_CODE
	{
		MC_MAIN = 0,
		MC_NOTIFY = 1,
		MC_TODO = 2,
	};
	enum MODEL_MAIN_VALUE
	{
		MMV_START = 0,
		MMV_EXIT = 1,
	};

	//value1为下列值时，value2为提醒ID
	enum MODEL_NOFIFY_VALUE
	{
		MNV_ADD = 0,
		MNV_DEL = 1,
		MNV_MODIFY = 2,
		MNV_DISPLAY = 3,
		MNV_CREATE_TABLE = 4,
	};
	//value1为下列值时，value2为计划ID
	enum MODEL_TODO_VALUE
	{
		MTV_ADD = 0,
		MTV_DEL = 1,
		MTV_MODIFY = 2,
		MTV_DISPLAY_LIST = 3,
		MTV_DISPLAY_ITEM = 4,
		MTV_CREATE_TABLE = 5,
		MTV_SETFINISH = 6,
		MTV_REACTIVE = 7,
		MTV_DELETEFOREVER = 8,
	};
};

#define LOG_MAIN(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_MAIN,LOG_CONST::MC_MAIN,v1,v2,remark)
#define LOG_NOTIFY(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_NOTIFY,LOG_CONST::MC_NOTIFY,v1,v2,remark)
#define LOG_TODO(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_TODO,LOG_CONST::MC_TODO,v1,v2,remark)
