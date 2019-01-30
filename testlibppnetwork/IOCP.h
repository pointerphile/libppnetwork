#pragma once
#include "NetworkObject.h"
#include "PacketMgr.h"
namespace PP {
	class IOCP : public NetworkObject
	{
	private:
		unsigned int m_uiNumThreads;
		HANDLE m_hIOCP;
		std::function<int()> m_fnProcessPacket;
	public:
		IOCP();
		~IOCP();
	public:
		virtual int Init() override;
		virtual int Run() override;
		virtual int Release() override;
	public:
		int SetProcessPacket(const std::function<int()>& fnParam);
		int SetIOCP();
		const HANDLE GetIOCPHandle();
	};
}