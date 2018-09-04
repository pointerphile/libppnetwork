#include "PPPacketPool.h"
#include "PPSessionManager.h"

PPPacketPool::PPPacketPool() {}
PPPacketPool::~PPPacketPool() {}

int PPPacketPool::Init() {
	this->SetEnable();

	return 0;
};
int PPPacketPool::Run() { return 0; };
int PPPacketPool::Release() {
	this->SetDisable();
	this->clear();

	return 0;
};

PP_PACKET& PPPacketPool::front() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	static PP_PACKET packetNull = { 0 };
	if (m_PacketList.size() && m_isEnable) {
		return m_PacketList.front();
	}
	else {
		return packetNull;
	}
}

bool PPPacketPool::push_back(UPACKET packet, PPSession* pSession) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_isEnable) {
		PP_PACKET temp;
		temp.m_packet = packet;
		temp.m_pSession = pSession;
		if (pSession == nullptr) {
			return false;
		}
		m_PacketList.push_back(temp);
	}

	return true;
}

bool PPPacketPool::push_back(PP_PACKET packet) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_isEnable) {
		if (packet.m_pSession == nullptr) {
			return false;
		}
		else {
			m_PacketList.push_back(packet);
			return true;
		}
	}
	
	return false;
}

bool PPPacketPool::pop_front() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_isEnable) {
		if (m_PacketList.empty()) {
			return false;
		}
		else {
			m_PacketList.front() = { 0 };
			m_PacketList.pop_front();
			return true;
		}
	}

	return false;
}

size_t PPPacketPool::size() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_isEnable) {
		return m_PacketList.size();
	}
	else {
		return -1;
	}
}

bool PPPacketPool::empty() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_isEnable) {
		m_PacketList.empty();
		return true;
	}
	else {
		return false;
	}
}

void PPPacketPool::clear() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	m_PacketList.clear();
}

void PPPacketPool::SetEnable() {
	m_isEnable = true;
}

void PPPacketPool::SetDisable() {
	m_isEnable = false;
}
