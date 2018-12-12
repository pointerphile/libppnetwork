#pragma once
#include "PPNetworkObject.h"
#include "PPIOCP.h"
namespace PP {
	class PPTCPIOCPServer : public PPNetworkObject {
	private:
		short m_iPort;			//Port number
		SOCKET m_socketListen;	//Listen Socket
		SOCKADDR_IN m_saListen;	//for Listen Socket
	private:
		PPIOCP IOCP;
	public:
		PPTCPIOCPServer();
		virtual ~PPTCPIOCPServer();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	};

	LIBPPNETWORK_API PP::PPTCPIOCPServer* GetServer();
}