#include "stdafx.h"
#include "TCPIOCPServer.h"


PP::TCPIOCPServer::TCPIOCPServer() :
	bEnable(false),
	m_socketListen(0),
	m_usPortNumber(0)
{
}


PP::TCPIOCPServer::~TCPIOCPServer()
{
}

int PP::TCPIOCPServer::Init()
{
	std::wcout << L"PP::TCPIOCPServer::Init()" << std::endl;
	WSADATA wsa;
	int iReturn = 0;
	//WSAStartup()
	iReturn = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iReturn != 0) {
		GetError(L"WSAStartup()");
		return iReturn;
	}
	//socket()
	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socketListen == INVALID_SOCKET)
	{
		GetError(L"WSASocket()");
		return iReturn;
	}
	//setsockopt
	bool bOptVal = true;
	int iOptLen = sizeof(bool);
	iReturn = setsockopt(m_socketListen, IPPROTO_TCP, TCP_NODELAY, (char *)&bOptVal, iOptLen);
	if (iReturn != 0) {
		GetError(L"setsockopt(TCP_NODELAY)");
		return iReturn;
	}
	//bind()
	m_saListen.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_saListen.sin_family = AF_INET;
	m_saListen.sin_port = htons(m_usPortNumber);
	iReturn = bind(m_socketListen, (sockaddr*)&m_saListen, sizeof(sockaddr));
	if (iReturn != 0) {
		GetError(L"bind()");
		return iReturn;
	}
	//listen()
	iReturn = listen(m_socketListen, SOMAXCONN);
	if (iReturn != 0) {
		GetError(L"listen()");
		return iReturn;
	}
	return 0;
}

int PP::TCPIOCPServer::Run()
{
	std::wcout << L"PP::TCPIOCPServer::Run()" << std::endl;
	while (true) {
		int iResult = 0;
		DWORD dwFlags = 0;
		PPSession Session = {};
		int addrlen = sizeof(Session.m_saSession);
		Session.m_ovRecv.dwFlag = ASYNCFLAG_RECV;
		Session.m_ovSend.dwFlag = ASYNCFLAG_SEND;

		Session.m_socketSession = WSAAccept(m_socketListen, (sockaddr*)&Session.m_saSession, &addrlen, nullptr, NULL);
		if (Session.m_socketSession == INVALID_SOCKET) {
			GetError(L"accept()");
			break;
		}

		//PPSessionManager::GetInstance().insert(Session.m_socketSession, Session);
		//auto iter = PPSessionManager::GetInstance().find(Session.m_socketSession);
		////CreateIoCompletionPort(): socket 정수 값을 IOCP의 키 값으로 사용함.
		//CreateIoCompletionPort((HANDLE)iter->second.m_socketSession, m_IOCP.GetIOCPHandle(), iter->second.m_socketSession, 0);

		//iter->second.m_wsabufRecv.buf = iter->second.m_bufRead;
		//iter->second.m_wsabufRecv.len = PACKET_BUFFER_SIZE;
		//iResult = WSARecv(iter->second.m_socketSession, &iter->second.m_wsabufRecv, 1, nullptr, &dwFlags, &iter->second.m_ovRecv, nullptr);
		//if (iResult == SOCKET_ERROR) {
		//	GetError(L"WSARecv()");
		//}
	}
	return 0;
}

int PP::TCPIOCPServer::Release()
{
	bEnable = false;
	return 0;
}

int PP::TCPIOCPServer::CheckPort(const short & usPortNumber)
{
	if (usPortNumber <= 1024) {
		return -1;
	}
	return 0;
}

int PP::TCPIOCPServer::SetPort(const short& usPortNumber)
{
	m_usPortNumber = usPortNumber;
	return 0;
}

int PP::TCPIOCPServer::Startup()
{
	int iReturn = 0;

	iReturn = CheckPort(m_usPortNumber);
	if (iReturn != 0) {
		return iReturn;
	}
	m_IOCP.Init();
	Init();
	bEnable = true;
	return 0;
}
