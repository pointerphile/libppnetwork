#pragma once
#include "PPInclude.h"

struct PPSession {
	PPOVERLAPPED m_ovRecv;
	PPOVERLAPPED m_ovSend;
	sockaddr_in m_saSession = {};
	char m_bufRead[2048] = {};
	char m_bufWrite[2048] = {};
	WSABUF m_wsabufRecv = {};
	WSABUF m_wsabufSend = {};
};