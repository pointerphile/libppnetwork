#pragma once
#include "PPInclude.h"

enum PacketType { PACKET_UNDEFINED, PACKET_SEND, PACKET_BROADCAST, PACKET_BROADCAST_EXCEPT_ME };

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
	std::string m_strUsername;
};

class PP_PACKET {
public:
	PacketType m_PacketType = PACKET_UNDEFINED;
	UPACKET m_packet;
	SOCKET m_socketSession;
};