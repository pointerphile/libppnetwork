#pragma once
#include "PPInclude.h"
#include "PPPacket.h"

class PPServerObject
{
protected:
	static bool m_isShutdown;
	std::thread m_threadThis;
	std::thread::id m_threadID;
	std::mutex static m_mutexThis;
	std::condition_variable m_cv;
public:
	PPServerObject();
	virtual ~PPServerObject();

	virtual int Init() = 0;
	virtual int Run() = 0;
	virtual int Release() = 0;

	int LaunchThread();
	void DisplayError(const wchar_t* strParam);
};

