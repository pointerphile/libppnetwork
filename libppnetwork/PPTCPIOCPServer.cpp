#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PPTCPIOCPServer.h"
#include "PPSessionManager.h"
#include "PPRecvPacketPoolServer.h"

PP::PPTCPIOCPServer::PPTCPIOCPServer() {}
PP::PPTCPIOCPServer::~PPTCPIOCPServer() {}

int PP::PPTCPIOCPServer::Init() {
	std::wcout << L"PPTCPIOCPServer::Init()..." << std::endl;
	OutputDebugStringW(L"PPTCPIOCPServer::Init()...\n");
	WSADATA wsa;
	int iReturn = 0;
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
	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socketListen == INVALID_SOCKET)
	{
		MessageBoxError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::wcout << L"setsockopt(TCP_NODELAY)..." << std::endl;
	OutputDebugStringW(L"setsockopt(TCP_NODELAY)...\n");
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_socketListen, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
	if (iReturn != 0) {
		DisplayError(L"setsockopt(TCP_NODELAY)");
		return iReturn;
	}
	//Check Port
	if (CheckPortNumber() != 0) {
		iReturn = Release();
		return -1;
	}
	//bind()
	std::wcout << L"bind()..." << std::endl;
	OutputDebugStringW(L"bind()...\n");
	m_saListen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_saListen.sin_family = AF_INET;
	m_saListen.sin_port = htons(m_iPort);
	iReturn = bind(m_socketListen, (sockaddr*)&m_saListen, sizeof(sockaddr));
	if (iReturn != 0) {
		DisplayError(L"bind()");
		return iReturn;
	}
	//listen()
	std::wcout << L"listen()..." << std::endl;
	OutputDebugStringW(L"listen()...\n");
	iReturn = listen(m_socketListen, SOMAXCONN);
	if (iReturn != 0) {
		DisplayError(L"listen()");
		return iReturn;
	}
	return 0;
}

int PP::PPTCPIOCPServer::Run() {
	std::wcout << L"accept()..." << std::endl;
	OutputDebugStringW(L"accept()...\n");
	while (true) {
		int iResult = 0;
		DWORD dwFlags = 0;
		PPSession Session = {};
		int addrlen = sizeof(Session.m_saSession);
		Session.m_ovRecv.dwFlag = ASYNCFLAG_RECV;
		Session.m_ovSend.dwFlag = ASYNCFLAG_SEND;
		
		Session.m_socketSession = WSAAccept(m_socketListen, (sockaddr*)&Session.m_saSession, &addrlen, nullptr, NULL);
		if (Session.m_socketSession == INVALID_SOCKET) {
			DisplayError(L"accept()");
			break;
		}
		
		PPSessionManager::GetInstance().insert(Session.m_socketSession, Session);
		auto iter = PPSessionManager::GetInstance().find(Session.m_socketSession);
		//CreateIoCompletionPort(): socket 정수 값을 IOCP의 키 값으로 사용함.
		CreateIoCompletionPort((HANDLE)iter->second.m_socketSession, m_IOCP.m_hIOCP, iter->second.m_socketSession, 0);

		iter->second.m_wsabufRecv.buf = iter->second.m_bufRead;
		iter->second.m_wsabufRecv.len = PACKET_BUFFER_SIZE;
		iResult = WSARecv(iter->second.m_socketSession, &iter->second.m_wsabufRecv, 1, nullptr, &dwFlags, &iter->second.m_ovRecv, nullptr);
		if (iResult == SOCKET_ERROR) {
			DisplayError(L"WSARecv()");
		}
	}
	return 0;
}

int PP::PPTCPIOCPServer::Release() {
	int iReturn = 0;

	PPSessionManager::GetInstance().clear();
	PPRecvPacketPoolServer::GetInstance().clear();

	if (m_socketListen != INVALID_SOCKET) {
		//shutdown()
		std::wcout << "shutdown()..." << std::endl;
		OutputDebugStringW(L"shutdown()...\n");
		iReturn = shutdown(m_socketListen, SD_BOTH);
		if (iReturn != 0) {
			if (WSAGetLastError() != WSAENOTCONN) {
				DisplayError(L"shutdown()");
			}
		}
		//closesocket()
		std::wcout << "closesocket()..." << std::endl;
		OutputDebugStringW(L"closesocket()...\n");
		iReturn = closesocket(m_socketListen);
		if (iReturn != 0) {
			DisplayError(L"closesocket()");
		}
	}
	m_socketListen = INVALID_SOCKET;
	//WSACleanup()
	std::wcout << "WSACleanup()..." << std::endl;
	OutputDebugStringW(L"WSACleanup()...\n");
	iReturn = WSACleanup();
	if (iReturn != 0) {
		DisplayError(L"WSACleanup()");
		return iReturn;
	}
	
	return 0;
}

int PP::PPTCPIOCPServer::CheckPortNumber() {
	if (m_iPort < 1024) {
		std::wcout << L"Insert port number 1024 ~ 65535" << std::endl;
		return -1;
	}
	return 0;
}

int PP::PPTCPIOCPServer::Startup() {
	int iReturn = 0;
	m_IOCP.SetNumberOfWorkers(m_iNumberOfThreads);
	m_IOCP.SetServer(true);
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

int PP::PPTCPIOCPServer::SetPortNumber(unsigned short iPort) {
	m_iPort = iPort;
	return 0;
}

int PP::PPTCPIOCPServer::SetNumberOfThreads(unsigned short iNumberOfThreads) {
	m_iNumberOfThreads = iNumberOfThreads;
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPServer::SetFP(std::function<int()> FP) {
	m_FP = FP;
	m_IOCP.SetFP(m_FP);
	return 0;
}

PP::PPTCPIOCPServer* PP::GetServer() {
	return new PP::PPTCPIOCPServer();
}
