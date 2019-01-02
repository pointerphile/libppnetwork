#pragma once

#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPRecvPacketPoolServer : public PPPacketPool, public PPSingleton<PPRecvPacketPoolServer> {
	private:
		friend class PPSingleton<PPRecvPacketPoolServer>;
	public:
		PPRecvPacketPoolServer();
		virtual ~PPRecvPacketPoolServer();
	};
}