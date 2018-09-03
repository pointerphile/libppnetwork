#pragma once
#include "PPServerObject.h"
#include "PPIOCP.h"
#include "PPSessionManager.h"

class PPAcceptor : public PPServerObject {
private:
	SOCKET m_socketListen;
	sockaddr_in m_saListen;
	short m_iPort;
public:
	PPAcceptor();
	virtual ~PPAcceptor();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
public:
	int Set(short iPort);
};

