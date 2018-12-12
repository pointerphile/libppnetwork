#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPIOCP : public PPNetworkObject {
	private:
		HANDLE m_hIOCP;
		int m_NumberOfWorkers = 0;//������ IOCP ��Ŀ ��
	public:
		PPIOCP();
		virtual ~PPIOCP();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		HANDLE BindSocket(HANDLE handle, ULONG_PTR CompletionKey);
	};
}

