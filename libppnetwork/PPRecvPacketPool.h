#pragma once
#include "PPNetworkObject.h"
#include "PPSingleton.h"

namespace PP {
	class PPRecvPacketPool : PPSingleton<PPRecvPacketPool> {
	private:
		friend class PPSingleton<PPRecvPacketPool>;
	public:
		std::list<PP::PPSendPacket> m_listRecvPacket;
	public:
		PPRecvPacketPool();
		virtual ~PPRecvPacketPool();
	};
}
