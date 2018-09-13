#pragma once
#include "PPInclude.h"
#include "PPPacket.h"

class PPServerObject
{
protected:
	static bool m_isShutdown;
	std::thread m_threadThis;
	std::thread::id m_threadID;
	std::mutex m_mutexThis;
public:
	PPServerObject();
	virtual ~PPServerObject();

	virtual int Init() = 0;
	virtual int Run() = 0;
	virtual int Release() = 0;

	int LaunchThread();
	void DisplayError(const wchar_t* strParam);
};

