#include "PPSessionManager.h"

PPSessionManager::PPSessionManager() {}
PPSessionManager::~PPSessionManager() {}

bool PPSessionManager::push_back(SOCKET socket, PPSession session) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	m_mapSession.insert(std::make_pair(socket, session));
	std::cout << "session : push_back()..." << std::endl;

	return true;
}

std::map<SOCKET, PPSession>::iterator PPSessionManager::begin() {
	std::lock_guard<std::mutex> lock(m_mutexThis);

	return m_mapSession.begin();
}

std::map<SOCKET, PPSession>::iterator PPSessionManager::end() {
	std::lock_guard<std::mutex> lock(m_mutexThis);

	return m_mapSession.end();
}

std::map<SOCKET, PPSession>::iterator PPSessionManager::find(SOCKET socket) {
	std::lock_guard<std::mutex> lock(m_mutexThis);

	return m_mapSession.find(socket);
}

bool PPSessionManager::erase(SOCKET socket) {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	if (m_mapSession.size() != 0) {
		shutdown(socket, SD_BOTH);
		closesocket(socket);
		m_mapSession.find(socket)->second = { 0 };
		m_mapSession.erase(socket);
		socket = 0;
		std::cout << "session : erase()..." << std::endl;
		return true;
	}
	else {
		std::cout << "session is empty" << std::endl;
		return false;
	}
	
	return false;
}

bool PPSessionManager::clear() {
	std::lock_guard<std::mutex> lock(m_mutexThis);
	for (auto &i : m_mapSession) {
		shutdown(i.second.m_socketSession, SD_BOTH);
		closesocket(i.second.m_socketSession);
	}
	m_mapSession.clear();

	return true;
}

size_t PPSessionManager::size() {
	std::lock_guard<std::mutex> lock(m_mutexThis);

	return m_mapSession.size();
}
