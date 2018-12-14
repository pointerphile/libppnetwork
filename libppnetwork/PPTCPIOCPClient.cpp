#include "PPTCPIOCPClient.h"

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
	m_socketClient = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socketClient == INVALID_SOCKET)
	{
		MessageBoxError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	std::wcout << L"setsockopt(TCP_NODELAY)..." << std::endl;
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_socketClient, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
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
	m_saClient.sin_addr.S_un.S_addr = inet_addr(m_strIPv4.c_str());
	m_saClient.sin_family = AF_INET;
	m_saClient.sin_port = htons(m_iPort);
	WSAConnect(m)
	iReturn = bind(m_socketClient, (sockaddr*)&m_saClient, sizeof(sockaddr));
	if (iReturn != 0) {
		DisplayError(L"connect()");
		return iReturn;
	}
	return 0;
}

int PP::PPTCPIOCPClient::Run() {
	return 0;
}

int PP::PPTCPIOCPClient::Release() {
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
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetPortNumber(unsigned short iPort) {
	m_iPort = iPort;
	return 0;
}

LIBPPNETWORK_API int PP::PPTCPIOCPClient::SetNumberOfThreads(unsigned short iNumberOfThreads) {
	return 0;
}
