#pragma once

#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPServerRecvPacketPool : public PPPacketPool, public PPSingleton<PPServerRecvPacketPool> {
	private:
		friend class PPSingleton<PPServerRecvPacketPool>;
	public:
		PPServerRecvPacketPool();
		virtual ~PPServerRecvPacketPool();
	};
}