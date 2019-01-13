#pragma once
#include "PPNetworkObject.h"
#include "PPReceiver.h"
#include "PPSender.h"

namespace PP {
	class PPIOCP : public PPNetworkObject {
	private:
		unsigned short m_iNumberOfThreads = 0;//积己且 IOCP 况目 荐
		PPReceiver m_Receiver;
		PPSender m_Sender;
		bool bIsServer;
	public:
		HANDLE m_hIOCP;
	public:
		PPIOCP();
		virtual ~PPIOCP();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	private:
		int DispatchRecv(PPSession Session, DWORD dwTransferred);
		int DispatchSend(PPSession Session, DWORD dwTransferred);
	public:
		int SetServer(bool boolean);
		int SetNumberOfWorkers(unsigned short iNumberOfThreads);
		HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
		//int(*m_FP)();
		//int SetFP(int(*FP)());
		std::function<int()> m_FP;
		int SetFP(std::function<int()> FP);
	};
}

