#include "PPPacketPool.h"
#include "PPSessionManager.h"

PPPacketPool::PPPacketPool() {}
PPPacketPool::~PPPacketPool() {}

PP_PACKET& PPPacketPool::front() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	static PP_PACKET packetNull = { 0 };
	if (m_PacketList.empty()) {
		return packetNull;
	}
	else {
		return m_PacketList.front();
	}
}

bool PPPacketPool::push_back(UPACKET packet, PPSession* pSession) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	//std::cout << "PPPacketPool : push_back(UPACKET, PPSession*)..." << packet.m_msg << std::endl;
	PP_PACKET temp;
	temp.m_packet = packet;
	temp.m_pSession = pSession;

	m_PacketList.push_back(temp);

	return true;
}

bool PPPacketPool::push_back(PP_PACKET packet) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	//std::cout << "PPPacketPool : push_back(packet) count " << m_PacketList.size() << " : " << packet.m_packet.m_msg << std::endl;

	m_PacketList.push_back(packet);

	return true;
}

bool PPPacketPool::pop_front() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_PacketList.empty()) {
		//std::cout << "PPPacketPool is empty..." << std::endl;
		return false;
	}
	else {
		m_PacketList.front() = { 0 };
		m_PacketList.pop_front();
		//std::cout << "PPPacketPool : pop_front()..." << std::endl;
		return true;
	}

	return true;
}

size_t PPPacketPool::size() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	return m_PacketList.size();
}

bool PPPacketPool::empty() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	return m_PacketList.empty();
}

void PPPacketPool::clear() {
	m_PacketList.clear();
}