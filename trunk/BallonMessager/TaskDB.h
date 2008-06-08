#pragma once
#include "ITask.h"
#include <atlapp.h>
#include <atlmisc.h>
using namespace ATL;
#include <vector>

#include "ThreadLock.h"

class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);

	//保存新任务时，task_id应当设为0.否则会覆盖旧的任务
	int AddTask(const ITask &task);
	bool AddDailyTask(int iHour, int iMinute, ATL::CString strMessage);
	bool RemoveTask(int idTask);

	void GetTaskList(std::vector<int> &vecIdOut);
	//ITask *GetTask(int idTask);
	bool GetTask(int idTask,ITask &task);
	bool FindTaskRunNow(ITask &task);
	bool UpdateTask(const ITask &task);
private:
	CThreadLock m_cs;
	std::vector<ITask> m_vecTask;
public:
	// 按任务类型及执行时间查询任务
	bool GetTask(CTime taskTime, int taskType, ITask &task);
	// 通过时间和类型查询任务,时、分用字符串表示
	bool GetTask(WTL::CString strHour, WTL::CString strMin, int taskType, ITask & task);
	// 通过时间查询任务，时、分用int方式表示
	bool GetTask(int iHour, int iMin, int taskType, ITask & task);
};

extern TaskDB g_TaskDB;
