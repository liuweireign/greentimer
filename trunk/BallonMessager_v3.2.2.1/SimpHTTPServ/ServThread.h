#pragma once
#include "Thread/jthread.h"

#include "./spserver/spiocpserver.hpp"

class ServThread :
	public JThread
{
public:
	ServThread();
	~ServThread(void);

	virtual void *Thread();

	void shutdown();
private:
	SP_IocpServer *m_pSpServer;
};
