#pragma once
#include "PPNetworkObject.h"
#include "PPIOCP.h"
namespace PP {
	class PPTCPIOCPServer : public PPNetworkObject {
	private:
		unsigned short m_iPort;	//Port number
		unsigned short m_iNumberOfThreads;
		SOCKET m_socketListen;	//Listen Socket
		SOCKADDR_IN m_saListen;	//for Listen Socket
	public:
		PPIOCP m_IOCP;
	public:
		PPTCPIOCPServer();
		virtual ~PPTCPIOCPServer();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		int CheckPortNumber();
		LIBPPNETWORK_API int Startup();
		LIBPPNETWORK_API int SetPortNumber(unsigned short iPort);
		LIBPPNETWORK_API int SetNumberOfThreads(unsigned short iNumberOfThreads);
		int(*m_FP)();
		LIBPPNETWORK_API int SetFP(int(*FP)());
	};

	LIBPPNETWORK_API PP::PPTCPIOCPServer* GetServer();
}