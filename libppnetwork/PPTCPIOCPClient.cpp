#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PPTCPIOCPClient.h"
#include "PPSessionManager.h"
#include "PPRecvPacketPoolClient.h"

PP::PPTCPIOCPClient::PPTCPIOCPClient() {}
PP::PPTCPIOCPClient::~PPTCPIOCPClient() {}

int PP::PPTCPIOCPClient::Init() {
	WSADATA wsa;
	int iReturn = 0;
	DWORD dwFlags = 0;
	//WSAStartup()
	std::wcout << L"WSAStartup()..." << std::endl;
	OutputDebugStringW(L"WSAStartup()...\n");
	iReturn = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iReturn != 0) {
		MessageBoxError(L"WSAStartup()");
		return iReturn;
	}
	//socket()
	std::wcout << L"WSASocket()..." << std::endl;
	OutputDebugStringW(L"WSASocket()...\n");
	m_Session.m_socketSession = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_Session.m_socketSession == INVALID_SOCKET)
	{
		MessageBoxError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::wcout << L"setsockopt(TCP_NODELAY)..." << std::endl;
	OutputDebugStringW(L"setsockopt(TCP_NODELAY)...\n");
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
	OutputDebugStringW(L"connect()...\n");
	m_Session.m_saSession.sin_addr.S_un.S_addr = inet_addr(m_strIPv4.c_str());
	m_Session.m_saSession.sin_family = AF_INET;
	m_Session.m_saSession.sin_port = htons(m_iPort);
	iReturn = WSAConnect(m_Session.m_socketSession, (sockaddr*)&m_Session.m_saSession, sizeof(m_Session.m_saSession), nullptr, nullptr, nullptr, nullptr);
	if (iReturn != 0) {
		MessageBoxError(L"WSAConnect()");
		return iReturn;
	}
	//GetIPv4Address
	GetIPv4Address();

	m_Session.m_ovRecv.dwFlag = ASYNCFLAG_RECV;
	m_Session.m_ovSend.dwFlag = ASYNCFLAG_SEND;

	//IOCP에 소켓 바인드
	PPSessionManager::GetInstance().insert(m_Session.m_socketSession, m_Session);
	auto iter = PPSessionManager::GetInstance().find(m_Session.m_socketSession);
	CreateIoCompletionPort((HANDLE)iter->second.m_socketSession, m_IOCP.m_hIOCP, iter->second.m_socketSession, 0);

	iter->second.m_wsabufRecv.buf = iter->second.m_bufRead;
	iter->second.m_wsabufRecv.len = PACKET_BUFFER_SIZE;
	iReturn = WSARecv(iter->second.m_socketSession, &iter->second.m_wsabufRecv, 1, nullptr, &dwFlags, &iter->second.m_ovRecv, nullptr);
	if (iReturn == SOCKET_ERROR) {
		DisplayError(L"WSARecv()");
	}

	return 0;
}

int PP::PPTCPIOCPClient::Run() {
	return 0;
}

int PP::PPTCPIOCPClient::Release() {
	int iReturn = 0;

	
	PPSessionManager::GetInstance().clear();
	PPRecvPacketPoolClient::GetInstance().clear();

	//WSACleanup()
	std::wcout << L"WSACleanup()..." << std::endl;
	OutputDebugStringW(L"WSACleanup()...\n");
	iReturn = WSACleanup();
	if (iReturn != 0) {
		DisplayError(L"WSACleanup()");
		return iReturn;
	}
	m_Session = {};
	m_IOCP.Release();
	return 0;
}

int PP::PPTCPIOCPClient::CheckPortNumber() {
	if (m_iPort < 1024) {
		std::wcout << L"Insert port number 1024 ~ 65535" << std::endl;
		MessageBoxW(nullptr, L"Insert port number 1024 ~ 65535", L"Error", 0);
		return -1;
	}
	return 0;
}

int PP::PPTCPIOCPClient::SetIPv4Address() {
	int iResult = 0;
	sockaddr_in saClient = {};
	int iSize = sizeof(saClient);
	iResult = getsockname(m_Session.m_socketSession, (sockaddr*)&saClient, &iSize);
	if (iResult != ERROR_SUCCESS) {
		DisplayError(L"getsockname()");
		return iResult;
	}
	m_strIPv4 = inet_ntoa(saClient.sin_addr);
	std::cout << "My IP: " << m_strIPv4.c_str() << std::endl;
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

LIBPPNETWORK_API std::string PP::PPTCPIOCPClient::GetIPv4Address() {
	return m_strIPv4;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetFP(std::function<int()> FP) {
	m_FP = FP;
	m_IOCP.SetFP(m_FP);
	return 0;
}

LIBPPNETWORK_API PP::PPTCPIOCPClient* PP::GetClient() {
	return new PP::PPTCPIOCPClient();
}