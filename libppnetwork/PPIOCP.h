#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPIOCP : public PPNetworkObject {
	private:
		unsigned short m_iNumberOfThreads = 0;//생성할 IOCP 워커 수
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
		int DispatchRecv(DWORD dwTransferred);
		int DispatchSend(DWORD dwTransferred);
		int SetNumberOfWorkers(unsigned short iNumberOfThreads);
		HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
	};
}

