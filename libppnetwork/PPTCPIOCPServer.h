#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPTCPIOCPServer : public PPNetworkObject {
	private:
		std::string m_strIPv4;
		short m_iPort;
		SOCKET m_socketListen;
		SOCKADDR_IN m_saListen;
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