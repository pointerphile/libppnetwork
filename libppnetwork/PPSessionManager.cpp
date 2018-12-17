#include "PPSessionManager.h"

PP::PPSessionManager::PPSessionManager() {}
PP::PPSessionManager::~PPSessionManager() {}

bool PP::PPSessionManager::insert(SOCKET socket, PPSession session) {
	m_mapSession.insert(std::make_pair(socket, session));
	std::wcout << L"PPSessionManager : push_back()..." << std::endl;
	OutputDebugStringW(L"PPSessionManager : push_back()...\n");
	return true;
}

bool PP::PPSessionManager::erase(SOCKET socket) {
	if (m_mapSession.empty()) {
		std::wcout << L"PPSessionManager: session list is empty." << std::endl;
		OutputDebugStringW(L"PPSessionManager: session list is empty.\n");
		return false;
	}
	else {
		auto iter = m_mapSession.find(socket);
		if (iter == m_mapSession.end()) {
			std::wcout << L"PPSessionManager: session is not in list." << std::endl;
			OutputDebugStringW(L"PPSessionManager: session is not in list.\n");
			return false;
		}
		shutdown(socket, SD_BOTH);
		closesocket(socket);
		iter->second = {};
		m_mapSession.erase(socket);
		socket = 0;
		std::wcout << L"PPSessionManager : erase()..." << std::endl;
		OutputDebugStringW(L"PPSessionManager : erase()...\n");
		return true;
	}
}

void PP::PPSessionManager::clear() {
	for (auto &iter : m_mapSession) {
		shutdown(iter.second.m_socketSession, SD_BOTH);
		closesocket(iter.second.m_socketSession);
		iter.second.m_socketSession = 0;
	}
	m_mapSession.clear();
}

std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::begin() {
	return m_mapSession.begin();
}

std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::end() {
	return m_mapSession.end();
}

const std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::find(SOCKET socket) {
	return m_mapSession.find(socket);
}

