#include "PPTCPIOCPServer.h"

PP::PPTCPIOCPServer::PPTCPIOCPServer() {}
PP::PPTCPIOCPServer::~PPTCPIOCPServer() {}

int PP::PPTCPIOCPServer::Init() {
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
	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socketListen == INVALID_SOCKET)
	{
		MessageBoxError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::wcout << L"setsockopt(TCP_NODELAY)..." << std::endl;
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_socketListen, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
	if (iReturn != 0) {
		MessageBoxError(L"setsockopt(TCP_NODELAY)");
		return iReturn;
	}
	//Check Port
	if (m_iPort < 49152) {
		std::wcout << L"Insert Port number 49152 ~ 65535" << std::endl;
		iReturn = Release();
		return -1;
	}
	//bind()
	std::wcout << L"bind()..." << std::endl;
	m_saListen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_saListen.sin_family = AF_INET;
	m_saListen.sin_port = htons(m_iPort);
	iReturn = bind(m_socketListen, (sockaddr*)&m_saListen, sizeof(sockaddr));
	if (iReturn != 0) {
		MessageBoxError(L"bind()");
		return iReturn;
	}
	//listen()
	std::wcout << L"listen()..." << std::endl;
	iReturn = listen(m_socketListen, SOMAXCONN);
	if (iReturn != 0) {
		MessageBoxError(L"listen()");
		return iReturn;
	}

	return 0;
}

int PP::PPTCPIOCPServer::Run() {
	while (true) {
		std::wcout << "PPTCPIOCPServer..." << std::endl;
		Sleep(1000);
	}
	return 0;
}

int PP::PPTCPIOCPServer::Release() {
	int iReturn = 0;

	if (m_socketListen != INVALID_SOCKET) {
		//shutdown()
		std::wcout << "shutdown()..." << std::endl;
		iReturn = shutdown(m_socketListen, SD_BOTH);
		if (iReturn != 0) {
			if (WSAGetLastError() != WSAENOTCONN) {
				MessageBoxError(L"shutdown()");
			}
		}
		//closesocket()
		std::wcout << "closesocket()..." << std::endl;
		iReturn = closesocket(m_socketListen);
		if (iReturn != 0) {
			MessageBoxError(L"closesocket()");
		}
	}
	m_socketListen = 0;
	//WSACleanup()
	std::wcout << "WSACleanup()..." << std::endl;
	iReturn = WSACleanup();
	if (iReturn != 0) {
		MessageBoxError(L"WSACleanup()");
		return iReturn;
	}

	return 0;
}

LIBPPNETWORK_API PP::PPTCPIOCPServer * PP::GetServer()
{
	return new PP::PPTCPIOCPServer();
}
