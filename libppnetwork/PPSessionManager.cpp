#include "PPSessionManager.h"
#include "PPSender.h"

PP::PPSessionManager::PPSessionManager() {}
PP::PPSessionManager::~PPSessionManager() {}

int PP::PPSessionManager::Init() { return 0; }
int PP::PPSessionManager::Run() { return 0; }
int PP::PPSessionManager::Release() { return 0; }

bool PP::PPSessionManager::insert(SOCKET socket, PPSession session) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	m_mapSession.insert(std::make_pair(socket, session));
	std::wcout << L"PPSessionManager : push_back()..." << socket << std::endl;
	OutputDebugStringW(L"PPSessionManager : push_back()...\n");
	return true;
}

bool PP::PPSessionManager::erase(SOCKET socket) {
	//std::lock_guard<std::mutex> lock(m_mutex);
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
		else {
			std::wcout << L"PPSessionManager : erasing..." << socket << std::endl;
			shutdown(socket, SD_BOTH);
			closesocket(socket);
			m_mapSession.erase(socket);
			std::wcout << L"PPSessionManager : erase()...now size():" << m_mapSession.size() << std::endl;
			OutputDebugStringW(L"PPSessionManager : erase()...\n");
			return true;
		}
	}
}

void PP::PPSessionManager::clear() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	for (auto &iter : m_mapSession) {
		shutdown(iter.second.m_socketSession, SD_BOTH);
		closesocket(iter.second.m_socketSession);
		iter.second.m_socketSession = 0;
	}
	m_mapSession.clear();
}

std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::begin() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_mapSession.begin();
}

std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::end() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_mapSession.end();
}

const std::map<SOCKET, PP::PPSession>::iterator PP::PPSessionManager::find(SOCKET socket) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_mapSession.find(socket);
}

size_t PP::PPSessionManager::size() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_mapSession.size();
}