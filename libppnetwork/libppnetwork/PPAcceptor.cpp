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
	// std::locale::global(std::locale("Korean"));
	//WSAStartup()
	std::cout << "WSAStartup()..." << std::endl;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iResult != 0) {
		DisplayError("WSAStartup()");
		return iResult;
	}
	//socket()
	std::cout << "socket()..." << std::endl;
	m_socketListen = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketListen == INVALID_SOCKET)
	{
		DisplayError("socket()");
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
		DisplayError("bind()");
		return iResult;
	}
	//listen()
	std::cout << "listen()..." << std::endl;
	iResult = listen(m_socketListen, SOMAXCONN);
	if (iResult != 0) {
		DisplayError("listen()");
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
		PPSession session = { 0 };
		int addrlen = sizeof(session.m_saClient);
		session.m_socketClient = accept(m_socketListen, (sockaddr*)&session.m_saClient, &addrlen);
		if (session.m_socketClient == INVALID_SOCKET) {
			DisplayError("accept()");
			break;
		}
		PPSessionManager::GetInstance().push_back(session.m_socketClient, session);
		PPSession* pSession = &PPSessionManager::GetInstance().find(session.m_socketClient)->second;
		//CreateIoCompletionPort()
		PPIOCP::GetInstance().BindSocket((HANDLE)pSession->m_socketClient, (ULONG_PTR)pSession);

		UPACKET packet;
		sprintf_s(packet.m_msg, "À¸¾Æ¾Ç");
		pSession->m_wsabufSend.buf = packet.m_msg;
		pSession->m_wsabufSend.len = (ULONG)strlen(packet.m_msg);

		iResult = WSASend(pSession->m_socketClient, &pSession->m_wsabufSend, 1, nullptr, 0, &pSession->m_ov, nullptr);
		if (iResult == SOCKET_ERROR) {
			DisplayError("WSASend()");
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
