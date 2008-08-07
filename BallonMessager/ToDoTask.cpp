#include "StdAfx.h"
#include ".\todotask.h"
using namespace std;

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

TodoSet::TodoSet()
{

}

bool TodoSet::Load()
{
	return true;
}


void TodoSet::GetTodoList( std::set<int> &taskIDs )
{
	set<ToDoTask>::const_iterator it =  m_setTask.begin();
	for (;it!=m_setTask.end();it++)
	{
		m_setTask.insert((*it).id);
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
	if (m_setTask.empty())
	{
		return 1;
	}
	int new_id = (*m_setTask.end()).id + 1;
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