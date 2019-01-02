#pragma once
#include "PPPacketPool.h"
#include "PPSingleton.h"

namespace PP {
	class PPRecvPacketPoolClient : public PPPacketPool, public PPSingleton<PPRecvPacketPoolClient> {
	private:
		friend class PPSingleton<PPRecvPacketPoolClient>;
	public:
		PPRecvPacketPoolClient();
		virtual ~PPRecvPacketPoolClient();
	};
}