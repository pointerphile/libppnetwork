#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PPConnector.h"
#include "PPSessionManager.h"

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
		DisplayError(_TEXT("WSAStartup()"));
		return iResult;
	}

	return 0;
}

int PPConnector::Run() {
	int iResult = 0;
	PPSession session = { 0 };
	int addrlen = sizeof(session.m_saSession);
	//socket()
	std::cout << "socket()..." << std::endl;
	session.m_socketSession = socket(AF_INET, SOCK_STREAM, 0);
	if (session.m_socketSession == INVALID_SOCKET)
	{
		DisplayError(_TEXT("socket()"));
		return iResult;
	}
	//connenct()
	std::cout << "connenct()..." << std::endl;
	session.m_saSession.sin_family = AF_INET;
	session.m_saSession.sin_port = htons(m_iPort);
	session.m_saSession.sin_addr.s_addr = inet_addr(m_strAddress.c_str());

	//std::cout << "Connecting to " << SERVER_IPV4 << ":" << SERVER_PORT << "..." << std::endl;
	iResult = connect(session.m_socketSession, (SOCKADDR*)&session.m_saSession, addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cout << "Connection failed." << std::endl;
		DisplayError(_TEXT("connect()"));
		system("pause");
		return -1;
	}
	else {
		std::cout << "Connected" << std::endl;
	}
	PPSessionManager::GetInstance().push_back(session.m_socketSession, session);
	PPSession* pSession = &PPSessionManager::GetInstance().find(session.m_socketSession)->second;
	//CreateIoCompletionPort()
	PPIOCP::GetInstance().BindSocket((HANDLE)pSession->m_socketSession, (ULONG_PTR)pSession);

	return 0;
}
int PPConnector::Release() {
	WSACleanup();

	return 0;
}
