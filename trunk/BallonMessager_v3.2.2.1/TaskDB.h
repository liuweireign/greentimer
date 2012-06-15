#pragma once
#include "ITask.h"
#include <atlapp.h>
#include <atlmisc.h>
using namespace ATL;
#include <vector>

#include "ThreadLock.h"

//任务数据库
//此对象是线程安全的
//请通过g_TaskDB使用此类。
class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);

	//从默认地址恢复任务数据
	bool ReadFromDB();
	//把任务数据写到默认文件中
	bool SaveToDB();
	
	//增加新任务
	bool AddTask(const ITask &task);
	//增加每日任务
	bool AddDailyTask(int iHour, int iMinute, ATL::CString strMessage);
	//增加一次性任务
	bool AddTimeTask(const CTime &tmRuntime, ATL::CString strMessage);
	//删除任务
	bool RemoveTask(int idTask);
	//更新任务信息（根据task中的ID来确定任务）
	bool UpdateTask(const ITask &task);

	//读取系统中的任务列表
	void GetTaskList(std::vector<int> &vecIdOut);
	//获取单个任务的信息
	bool GetTask(int idTask,ITask &task);
	// 按任务类型及执行时间查询任务
	bool GetTask(CTime taskTime, int taskType, ITask &task);
	// 通过时间和类型查询任务,时、分用字符串表示
	bool GetTask(ATL::CString strHour, ATL::CString strMin, int taskType, ITask & task);
	// 通过时间查询任务，时、分用int方式表示
	bool GetTask(int iHour, int iMin, int taskType, ITask & task);


	//查询：现在这个时刻，是否有任务需要执行？
	bool FindTaskRunNow(ITask &task);
private:
	CThreadLock m_cs;
	std::vector<ITask> m_vecTask;
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);
};

extern TaskDB g_TaskDB;
