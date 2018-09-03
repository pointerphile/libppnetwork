#pragma once
#include "PPAcceptor.h"
#include "PPIOCP.h"

class PPIOCPTCPServer : public PPServerObject {
public:
	PPAcceptor Acceptor;
	PPSender Sender;
	int m_iThread;
public:
	PPIOCPTCPServer();
	virtual ~PPIOCPTCPServer();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
public:
	int Startup(short iPort, short iThread);
	int Shutdown();
};

