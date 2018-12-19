#pragma once
#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPRecvPacketPool : public PPPacketPool, public PPSingleton<PPRecvPacketPool> {
	private:
		friend class PPSingleton<PPRecvPacketPool>;
	public:
		PPRecvPacketPool();
		virtual ~PPRecvPacketPool();
	};
}
