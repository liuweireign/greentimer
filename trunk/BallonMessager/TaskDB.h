#pragma once
#include "ITask.h"
#include <vector>


class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);

	//保存新任务时，task_id应当设为0.否则会覆盖旧的任务
	int AddTask(const ITask &task);
	bool RemoveTask(int idTask);

	void GetTaskList(std::vector<int> &vecIdOut);
	ITask *GetTask(int idTask);

	ITask *TaskRunNow();
private:
	std::vector<ITask> m_vecTask;
};
