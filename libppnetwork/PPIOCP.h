#pragma once
#include "PPNetworkObject.h"
#include "PPReceiver.h"
#include "PPSender.h"

namespace PP {
	class PPIOCP : public PPNetworkObject {
	private:
		unsigned short m_iNumberOfThreads = 0;//积己且 IOCP 况目 荐
		PPReceiver Receiver;
		PPSender Sender;
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
		int DispatchRecv(PPSession& Session, DWORD dwTransferred);
		int DispatchSend(PPSession& Session, DWORD dwTransferred);
		int SetNumberOfWorkers(unsigned short iNumberOfThreads);
		HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
		int(*m_FP)();
		LIBPPNETWORK_API int SetFP(int(*FP)());
	};
}

