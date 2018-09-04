#pragma once
//#include "PPPacket.h"
#include "PPServerObject.h"
class PPPacketPool : public PPServerObject {
private:
	bool m_isEnable;
	std::list<PP_PACKET> m_PacketList;
public:
	PPPacketPool();
	virtual ~PPPacketPool();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
public:
	PP_PACKET& front();
	bool push_back(UPACKET packet, PPSession* pSession);
	bool push_back(PP_PACKET packet);
	bool pop_front();
	size_t size();
	bool empty();
	void clear();
	void SetEnable();
	void SetDisable();
};

