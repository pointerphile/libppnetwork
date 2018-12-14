#pragma once
#include "PPNetworkObject.h"
#include "PPIOCP.h"
namespace PP {
	class PPTCPIOCPClient :	public PPNetworkObject {
	private:
		std::string m_strIPv4;
		unsigned short m_iPort;	//Port number
		unsigned short m_iNumberOfThreads;
		SOCKET m_socketClient;	//Listen Socket
		SOCKADDR_IN m_saClient;	//for Listen Socket
	private:
		PPIOCP m_IOCP;
	public:
		PPTCPIOCPClient();
		virtual ~PPTCPIOCPClient();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		int CheckPortNumber();
		LIBPPNETWORK_API int Startup();
		LIBPPNETWORK_API int SetHost(std::string strHostIPv4);
		LIBPPNETWORK_API int SetPortNumber(unsigned short iPort);
		LIBPPNETWORK_API int SetNumberOfThreads(unsigned short iNumberOfThreads);
	};
}