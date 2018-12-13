#include "PPSessionManager.h"

PP::PPSessionManager::PPSessionManager() {}
PP::PPSessionManager::~PPSessionManager() {}

bool PP::PPSessionManager::insert(SOCKET socket, PPSession session) {
	m_mapSession.insert(std::make_pair(socket, session));
	std::cout << "session : push_back()..." << std::endl;
	return true;
}

bool PP::PPSessionManager::erase(SOCKET socket) {
	if (m_mapSession.empty()) {
		std::wcout << L"PPSessionManager: session list is empty." << std::endl;
		return false;
	}
	else {
		auto iter = m_mapSession.find(socket);
		if (iter == m_mapSession.end()) {
			std::wcout << L"PPSessionManager: session is not in list." << std::endl;
			return false;
		}
		shutdown(socket, SD_BOTH);
		closesocket(socket);
		iter->second = {};
		m_mapSession.erase(socket);
		socket = 0;
		std::cout << "session : erase()..." << std::endl;
		return true;
	}
}

const std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::begin() {
	return m_mapSession.begin();
}

const std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::end() {
	return m_mapSession.end();
}

const std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::find(SOCKET socket) {
	return m_mapSession.find(socket);
}

