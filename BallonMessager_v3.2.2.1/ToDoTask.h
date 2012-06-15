#pragma once
#include <string>
#include <set>
#include <atltime.h>

//任务记录
class ToDoTask
{
public:
	//特殊任务ID
	enum {
		ERROR_TASKID = 0,
	};
	//任务优先级
	enum TaskPriority
	{
		TP_MOST_IMPORTANT = 0,
		TP_IMPORTANT = 1,
		TP_NORMAL = 2,
		TP_NOT_IMPORTANT = 3,
	};
	//任务状态
	enum TaskState
	{
		TS_NOT_START = 0,
		TS_WORKING = 1,
		TS_STOP = 2,
		TS_FINISHED = 3,
		TS_CANCEL = 4,

		//"删除"状态
		TS_DELETED = 5,
	};
	static TCHAR *PriorityText(TaskPriority priority);
	static TCHAR *StateText(TaskState state);
	static COLORREF GetStateColor(TaskState state);

	ToDoTask(void);
	ToDoTask(int taskid);
	~ToDoTask(void);
	bool operator < (const ToDoTask &task) const;

	//任务id
	int id;
	//任务标题
	std::string strTask;
	//优先级
	TaskPriority priority;
	//状态
	TaskState state;
	//评论
	std::string strRemark;

	//任务创建时间
	CTime tmCreateTime;
	//任务开始工作的时间
	CTime tmStartWorkTime;

	CTime tmPlanFinshTime;

	//任务操作历史
	//std::map<CTime,std::string> mapHistory;
};

//任务数据库
class TodoSet
{
public:
	TodoSet();

	//bool Load();
	//bool Save();

	//bool Load(const TCHAR *strDB);
	//bool Save(const TCHAR *strDB);
	void GetTodoList(std::set<int> &taskIDs);
	void GetHistoryTodoList(std::set<int> &taskIDs);
	ToDoTask GetToDo(int id);
	bool UpdateToDo(const ToDoTask &task);
	int AddToDo();
	bool DeleteToDo(int id);
	bool ReactiveToDo(int id);
	bool DeleteToDoForever(int id);
	//获取统计信息
	bool GetStatic(int &iFinished,int &iPlaned, int &iWorking);
	int GetTaskCount(int iState);	//按状态读任务总数。iState=-1代表取所有任务总数。出错则返回-1
private:
	std::string m_strDB;
private:
	bool CheckDBValid();
	void UpdateTaskFinishTime(int id,const CTime &tm);

	//将t_todo中的数据迁移到t_todo2中，然后删除t_todo。
	//由于t_todo版本还没发布，所以不需要发布这个函数。
	bool RecoverDataFromV1();
};

extern TodoSet g_todoSet;

