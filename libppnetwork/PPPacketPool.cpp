#include "PPPacketPool.h"

PP::PPPacketPool::PPPacketPool() {}
PP::PPPacketPool::~PPPacketPool() {}

LIBPPNETWORK_API PP::PPPacketForProcess& PP::PPPacketPool::front() {
	return m_listPacket.front();
}

LIBPPNETWORK_API void PP::PPPacketPool::push_back(PPPacketForProcess packet) {
	m_listPacket.push_back(packet);;
}

LIBPPNETWORK_API void PP::PPPacketPool::pop_front()
{
	m_listPacket.pop_front();
}

LIBPPNETWORK_API size_t PP::PPPacketPool::size()
{
	return m_listPacket.size();
}

LIBPPNETWORK_API bool PP::PPPacketPool::empty()
{
	return m_listPacket.empty();
}

LIBPPNETWORK_API void PP::PPPacketPool::clear()
{
	return m_listPacket.clear();
}
