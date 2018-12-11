#include "PPTCPIOCPServer.h"

PP::PPTCPIOCPServer::PPTCPIOCPServer() {}
PP::PPTCPIOCPServer::~PPTCPIOCPServer() {}

int PP::PPTCPIOCPServer::Init() {
	WSADATA wsa;
	int iReturn = 0;
	//WSAStartup()
	std::cout << "WSAStartup()..." << std::endl;
	iReturn = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iReturn != 0) {
		DisplayError(L"WSAStartup()");
		return iReturn;
	}
	//socket()
	std::cout << "WSASocket()..." << std::endl;
	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socketListen == INVALID_SOCKET)
	{
		DisplayError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::cout << "setsockopt(TCP_NODELAY)..." << std::endl;
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_socketListen, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
	if (iReturn != 0) {
		DisplayError(L"setsockopt(TCP_NODELAY)");
		return iReturn;
	}
	//bind()
	std::cout << "bind()..." << std::endl;
	m_saListen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_saListen.sin_family = AF_INET;
	m_saListen.sin_port = htons(m_iPort);
	iReturn = bind(m_socketListen, (sockaddr*)&m_saListen, sizeof(sockaddr));
	if (iReturn != 0) {
		DisplayError(L"bind()");
		return iReturn;
	}
	//listen()
	std::cout << "listen()..." << std::endl;
	iReturn = listen(m_socketListen, SOMAXCONN);
	if (iReturn != 0) {
		DisplayError(L"listen()");
		return iReturn;
	}

	return 0;
}

int PP::PPTCPIOCPServer::Run() {
	while (true) {
		std::wcout << "PPTCPIOCPServer¾Æ¾Æ¾Æ" << std::endl;
		Sleep(1000);
	}
	return 0;
}

int PP::PPTCPIOCPServer::Release() {
	shutdown(m_socketListen, SD_BOTH);
	closesocket(m_socketListen);
	m_socketListen = 0;
	WSACleanup();

	return 0;
}

LIBPPNETWORK_API PP::PPTCPIOCPServer * PP::GetServer()
{
	return new PP::PPTCPIOCPServer();
}
