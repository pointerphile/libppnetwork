#pragma once
#include "PPServerObject.h"
#include "PPIOCPWorker.h"
class PPIOCP : public PPServerObject, public PPSingleton<PPIOCP>
{
private:
	friend class PPSingleton<PPIOCP>;
	int m_hardware_concurrency = 0;
	std::list<PPIOCPWorker> m_listWorkers;
public:
	HANDLE m_hIOCP;
public:
	PPIOCP();
	virtual ~PPIOCP();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
public:
	int Set(short iThread);
	HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
};

