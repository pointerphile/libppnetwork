#include "PPPacketPool.h"

PP::PPPacketPool::PPPacketPool() {}
PP::PPPacketPool::~PPPacketPool() {}

LIBPPNETWORK_API PP::PPPacketForProcess& PP::PPPacketPool::front() {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_listPacket.front();
}

LIBPPNETWORK_API void PP::PPPacketPool::push_back(PPPacketForProcess packet) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listPacket.push_back(packet);;
}

LIBPPNETWORK_API void PP::PPPacketPool::pop_front() {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listPacket.pop_front();
}

LIBPPNETWORK_API size_t PP::PPPacketPool::size() {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_listPacket.size();
}

LIBPPNETWORK_API bool PP::PPPacketPool::empty() {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_listPacket.empty();
}

LIBPPNETWORK_API void PP::PPPacketPool::clear() {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_listPacket.clear();
}
