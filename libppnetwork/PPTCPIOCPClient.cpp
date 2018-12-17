#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PPTCPIOCPClient.h"
#include "PPSessionManager.h"

PP::PPTCPIOCPClient::PPTCPIOCPClient() {}
PP::PPTCPIOCPClient::~PPTCPIOCPClient() {}

int PP::PPTCPIOCPClient::Init() {
	WSADATA wsa;
	int iReturn = 0;
	//WSAStartup()
	std::wcout << L"WSAStartup()..." << std::endl;
	iReturn = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iReturn != 0) {
		MessageBoxError(L"WSAStartup()");
		return iReturn;
	}
	//socket()
	std::wcout << L"WSASocket()..." << std::endl;
	m_Session.m_socketSession = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_Session.m_socketSession == INVALID_SOCKET)
	{
		MessageBoxError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::wcout << L"setsockopt(TCP_NODELAY)..." << std::endl;
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_Session.m_socketSession, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
	if (iReturn != 0) {
		DisplayError(L"setsockopt(TCP_NODELAY)");
		return iReturn;
	}
	//Check Port
	if (CheckPortNumber() != 0) {
		iReturn = Release();
		return -1;
	}
	//connect()
	std::wcout << L"connect()..." << std::endl;
	m_Session.m_saSession.sin_addr.S_un.S_addr = inet_addr(m_strIPv4.c_str());
	m_Session.m_saSession.sin_family = AF_INET;
	m_Session.m_saSession.sin_port = htons(m_iPort);
	iReturn = WSAConnect(m_Session.m_socketSession, (sockaddr*)&m_Session.m_saSession, sizeof(m_Session.m_saSession), nullptr, nullptr, nullptr, nullptr);
	if (iReturn != 0) {
		DisplayError(L"WSAConnect()");
		return iReturn;
	}

	PPSessionManager::GetInstance().insert(m_Session.m_socketSession, m_Session);
	auto iter = PPSessionManager::GetInstance().find(m_Session.m_socketSession);
	CreateIoCompletionPort((HANDLE)iter->second.m_socketSession, m_IOCP.m_hIOCP, iter->second.m_socketSession, 0);
	return 0;
}

int PP::PPTCPIOCPClient::Run() {
	return 0;
}

int PP::PPTCPIOCPClient::Release() {
	int iReturn = 0;

	//WSACleanup()
	std::wcout << "WSACleanup()..." << std::endl;
	iReturn = WSACleanup();
	if (iReturn != 0) {
		DisplayError(L"WSACleanup()");
		return iReturn;
	}
	return 0;
}

int PP::PPTCPIOCPClient::CheckPortNumber() {
	if (m_iPort < 1024) {
		std::wcout << L"Insert port number 1024 ~ 65535" << std::endl;
		return -1;
	}
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::Startup() {
	int iReturn = 0;
	m_IOCP.SetNumberOfWorkers(m_iNumberOfThreads);
	iReturn = m_IOCP.Init();
	if (iReturn != 0) {
		return iReturn;
	}
	iReturn = Init();
	if (iReturn != 0) {
		return iReturn;
	}
	this->LaunchThread();
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetHost(std::string strHostIPv4) {
	m_strIPv4 = strHostIPv4;
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetPortNumber(unsigned short iPort) {
	m_iPort = iPort;
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetNumberOfThreads(unsigned short iNumberOfThreads) {
	m_iNumberOfThreads = iNumberOfThreads;
	return 0;
}

LIBPPNETWORK_API PP::PPTCPIOCPClient* PP::GetClient() {
	return new PP::PPTCPIOCPClient();
}