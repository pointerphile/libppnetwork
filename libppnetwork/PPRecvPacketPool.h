#pragma once
#include "PPNetworkObject.h"
#include "PPSingleton.h"

namespace PP {
	class PPRecvPacketPool : public PPSingleton<PPRecvPacketPool> {
	private:
		friend class PPSingleton<PPRecvPacketPool>;
	public:
		std::list<PP::PPRecvPacket> m_listRecvPacket;
	public:
		PPRecvPacketPool();
		virtual ~PPRecvPacketPool();
	};
}
