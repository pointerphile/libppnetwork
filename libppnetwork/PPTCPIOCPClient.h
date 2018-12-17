#pragma once
#include "PPNetworkObject.h"
#include "PPIOCP.h"
#include "PPSender.h"

namespace PP {
	class PPTCPIOCPClient :	public PPNetworkObject {
	private:
		std::string m_strIPv4;
		unsigned short m_iPort;	//Port number
		unsigned short m_iNumberOfThreads;
	private:
		PPSession m_Session;
		PPIOCP m_IOCP;
		PPSender m_Sender;
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

	LIBPPNETWORK_API PP::PPTCPIOCPClient* GetClient();
}