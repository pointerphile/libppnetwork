#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPIOCP : public PPNetworkObject {
	private:
		HANDLE m_hIOCP;
		int m_NumberOfWorkers = 0;//생성할 IOCP 워커 수
	public:
		PPIOCP();
		virtual ~PPIOCP();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
		int DispatchRecv(DWORD dwTransferred);
		int DispatchSend(DWORD dwTransferred);
	};
}

