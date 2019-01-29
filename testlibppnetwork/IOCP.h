#pragma once
#include "NetworkObject.h"
#include "PacketMgr.h"
namespace PP {
	class IOCP : public NetworkObject
	{
	private:
		std::function<int()> m_fnProcessPacket;
	public:
		IOCP();
		~IOCP();
	public:
		virtual int Run() override;
	public:
		int SetProcessPacket(const std::function<int()>& fnParam);
	};
}