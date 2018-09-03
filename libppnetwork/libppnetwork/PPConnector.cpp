#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PPConnector.h"

PPConnector::PPConnector() {}
PPConnector::~PPConnector() {}

int PPConnector::Set(std::string strAddress, short iPort) {
	m_strAddress = strAddress;
	m_iPort = iPort;

	return 0;
}

int PPConnector::Init()
{
	WSADATA wsa;
	int iResult = 0;
	std::locale::global(std::locale("Korean"));
	//WSAStartup()
	std::cout << "WSAStartup()..." << std::endl;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iResult != 0) {
		DisplayError("WSAStartup()");
		return iResult;
	}
	//socket()
	std::cout << "socket()..." << std::endl;
	m_socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketClient == INVALID_SOCKET)
	{
		DisplayError("socket()");
		return iResult;
	}
	return 0;
}

int PPConnector::Run() {
	//connenct()
	int iResult = 0;
	std::cout << "connenct()..." << std::endl;
	SOCKADDR_IN saServer;
	saServer = { 0 };
	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(m_iPort);
	saServer.sin_addr.s_addr = inet_addr(m_strAddress.c_str());
	//std::cout << "Connecting to " << SERVER_IPV4 << ":" << SERVER_PORT << "..." << std::endl;
	iResult = connect(m_socketClient, (SOCKADDR*)&saServer, sizeof(saServer));
	if (iResult == SOCKET_ERROR) {
		std::cout << "Connection failed." << std::endl;
		system("pause");
		return -1;
	}
	else {
		std::cout << "Connected" << std::endl;
	}

	return 0;
}
int PPConnector::Release() {
	shutdown(m_socketClient, SD_BOTH);
	closesocket(m_socketClient);
	m_socketClient = 0;
	WSACleanup();

	return 0;
}
