#pragma once
#include "PPNetworkObject.h"
#include "PPSingleton.h"

namespace PP {
	class PPSendPacketPool : public PPSingleton<PPSendPacketPool> {
	private:
		friend class PPSingleton<PPSendPacketPool>;
	public:
		std::list<PP::PPSendPacket> m_listSendPacket;
	public:
		PPSendPacketPool();
		virtual ~PPSendPacketPool();
	};
}