#pragma once
#include "PPInclude.h"

class PPSession {
public:
	PPOVERLAPPED m_ovRecv;
	PPOVERLAPPED m_ovSend;
	SOCKET m_socketSession = 0;
	sockaddr_in m_saSession = {};
	char m_bufRead[2048] = {};
	char m_bufWrite[2048] = {};
	WSABUF m_wsabufRecv = {};
	WSABUF m_wsabufSend = {};
	bool m_isAccoutAck = false;
	std::string m_strUsername;
};

class PP_PACKET {
public:
	UPACKET m_packet;
	SOCKET m_socketSession;
};
