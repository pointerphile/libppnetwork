#include "PPIOCP.h"



PPIOCP::PPIOCP()
{
}


PPIOCP::~PPIOCP()
{
}

int PPIOCP::Set(short iThread) {
	m_hardware_concurrency = iThread;

	return 0;
}

int PPIOCP::Init()
{
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, m_hardware_concurrency);
	//Launch IOCPWorker
	for (int i = 0; i < m_hardware_concurrency; i++) {
		m_listWorkers.emplace_back();
		m_listWorkers.back().LaunchThread();
	}

	return 0;
}

int PPIOCP::Run()
{
	return 0;
}

int PPIOCP::Release()
{
	CloseHandle(m_hIOCP);
	m_listWorkers.clear();
	
	return 0;
}

HANDLE PPIOCP::BindSocket(HANDLE handle, ULONG_PTR CompletionKey)
{
	return CreateIoCompletionPort(handle, m_hIOCP, CompletionKey, 0);
}
