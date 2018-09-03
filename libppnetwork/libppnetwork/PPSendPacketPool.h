#pragma once
#include "PPPacketPool.h"
class PPSendPacketPool : public PPPacketPool, public PPSingleton<PPSendPacketPool> {
private:
	friend class PPSingleton<PPSendPacketPool>;
public:
	PPSendPacketPool();
	virtual ~PPSendPacketPool();
};

