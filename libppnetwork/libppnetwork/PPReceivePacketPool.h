#pragma once
#include "PPPacketPool.h"
class PPReceivePacketPool : public PPPacketPool, public PPSingleton<PPReceivePacketPool> {
private:
	friend class PPSingleton<PPReceivePacketPool>;
public:
	PPReceivePacketPool();
	virtual ~PPReceivePacketPool();
};

