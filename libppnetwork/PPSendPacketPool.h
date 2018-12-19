#pragma once
#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPSendPacketPool : public PPPacketPool, public PPSingleton<PPSendPacketPool> {
	private:
		friend class PPSingleton<PPSendPacketPool>;
	public:
		PPSendPacketPool();
		virtual ~PPSendPacketPool();
	};
}