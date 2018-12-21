#pragma once
#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPClientRecvPacketPool : public PPPacketPool, public PPSingleton<PPClientRecvPacketPool> {
	private:
		friend class PPSingleton<PPClientRecvPacketPool>;
	public:
		PPClientRecvPacketPool();
		virtual ~PPClientRecvPacketPool();
	};
}