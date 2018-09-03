#pragma once
#include "PPInclude.h"
class PPSession {
public:
	enum mode { MODE_RECV, MODE_SEND };
	OVERLAPPED m_ov;
	SOCKET m_socketClient = 0;
	sockaddr_in m_saClient = { 0 };
	mode m_modeSession = MODE_RECV;
	char m_bufRecv[2048] = { 0 };
	char m_bufSend[2048] = { 0 };
	WSABUF m_wsabufRecv = { 0 };
	WSABUF m_wsabufSend = { 0 };
	bool m_isAccoutAck = false;
	std::string m_strSessionName;
};

class PP_PACKET {
public:
	UPACKET m_packet;
	PPSession* m_pSession;
};
