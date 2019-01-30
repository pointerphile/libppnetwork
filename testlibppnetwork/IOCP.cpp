#include "stdafx.h"
#include "IOCP.h"


PP::IOCP::IOCP() :
	m_hIOCP(NULL)
{
	m_uiNumThreads = std::thread::hardware_concurrency();
}


PP::IOCP::~IOCP()
{
	Release();
}

int PP::IOCP::Init()
{
	int iReturn = 0;
	std::wcout << L"PP::IOCP::IOCP()" << std::endl;

	iReturn = SetIOCP();
	if (iReturn != 0) {
		return iReturn;
	}
	return 0;
}

int PP::IOCP::Run()
{
	return 0;
}

int PP::IOCP::Release()
{
	if (m_hIOCP != NULL) {
		CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
	}
	return 0;
}

int PP::IOCP::SetProcessPacket(const std::function<int()>& fnParam)
{
	m_fnProcessPacket = fnParam;
	return 0;
}

int PP::IOCP::SetIOCP()
{
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, m_uiNumThreads);
	if (m_hIOCP == NULL) {
		return -1;
	}
	return 0;
}

const HANDLE PP::IOCP::GetIOCPHandle()
{
	return m_hIOCP;
}
