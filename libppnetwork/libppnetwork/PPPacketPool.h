#pragma once
//#include "PPPacket.h"
#include "PPServerObject.h"
class PPPacketPool : public PPServerObject {
private:
	std::list<PP_PACKET> m_PacketList;
public:
	PPPacketPool();
	virtual ~PPPacketPool();
public:
	virtual int Init() { return 0; };
	virtual int Run() { return 0; };
	virtual int Release() { return 0; };
public:
	PP_PACKET& front();
	bool push_back(UPACKET packet, PPSession* pSession);
	bool push_back(PP_PACKET packet);
	bool pop_front();
	size_t size();
	bool empty();
	void clear();
};

