#include "PPAcceptor.h"
#include "PPSessionManager.h"

PPAcceptor::PPAcceptor() {}
PPAcceptor::~PPAcceptor() {}

int PPAcceptor::Set(short iPort) {
	m_iPort = iPort;

	return 0;
}

int PPAcceptor::Init() {
	WSADATA wsa;
	int iResult = 0;
	std::locale::global(std::locale("Korean"));
	//WSAStartup()
	std::cout << "WSAStartup()..." << std::endl;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iResult != 0) {
		DisplayError(_TEXT("WSAStartup()"));
		return iResult;
	}
	//socket()
	std::cout << "socket()..." << std::endl;
	m_socketListen = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketListen == INVALID_SOCKET)
	{
		DisplayError(_TEXT("socket()"));
		return iResult;
	}
	//setsockopt
	//std::cout << "setsockopt(TCP_NODELAY)..." << std::endl;
	//bool bOptVal = true;
	//int bOptLen = sizeof(bool);
	//iResult = setsockopt(m_socketListen, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, bOptLen);
	//if (iResult != 0) {
	//	DisplayError("setsockopt(TCP_NODELAY)");
	//	return iResult;
	//}
	//bind()
	std::cout << "bind()..." << std::endl;
	m_saListen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_saListen.sin_family = AF_INET;
	m_saListen.sin_port = htons(m_iPort);
	iResult = bind(m_socketListen, (sockaddr*)&m_saListen, sizeof(sockaddr));
	if (iResult != 0) {
		DisplayError(_TEXT("bind()"));
		return iResult;
	}
	//listen()
	std::cout << "listen()..." << std::endl;
	iResult = listen(m_socketListen, SOMAXCONN);
	if (iResult != 0) {
		DisplayError(_TEXT("listen()"));
		return iResult;
	}
	
	return 0;
}

int PPAcceptor::Run() {
	//accept()
	std::cout << "accept()..." << std::endl;
	while (!m_isShutdown) {
		int iResult = 0;
		DWORD flags = 0;
		PPSession Session = {};
		Session.m_ovRecv.dwFlag = ASYNCFLAG_RECV;
		Session.m_ovSend.dwFlag = ASYNCFLAG_SEND;

		int addrlen = sizeof(Session.m_saSession);
		Session.m_socketSession = accept(m_socketListen, (sockaddr*)&Session.m_saSession, &addrlen);
		if (Session.m_socketSession == INVALID_SOCKET) {
			DisplayError(L"accept()");
			break;
		}
		PPSessionManager::GetInstance().insert(Session.m_socketSession, Session);
		std::map<SOCKET, PPSession>::iterator iter = PPSessionManager::GetInstance().find(Session.m_socketSession);
		//CreateIoCompletionPort()
		CreateIoCompletionPort((HANDLE)iter->second.m_socketSession, PPIOCP::GetInstance().m_hIOCP, iter->second.m_socketSession, 0);
		
		iter->second.m_wsabufRecv.buf = iter->second.m_bufRead;
		iter->second.m_wsabufRecv.len = BUFFER_SIZE;

		iResult = WSARecv(iter->second.m_socketSession, &iter->second.m_wsabufRecv, 1, nullptr, &flags, &iter->second.m_ovRecv, nullptr);
		if (iResult == SOCKET_ERROR) {
			DisplayError(L"WSARecv()");
		}
	}
	return 0;
}

int PPAcceptor::Release() {
	shutdown(m_socketListen, SD_BOTH);
	closesocket(m_socketListen);
	m_socketListen = 0;
	WSACleanup();

	return 0;
}
