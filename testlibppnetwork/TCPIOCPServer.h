#pragma once
#include "NetworkObject.h"
#include "IOCP.h"

namespace PP{
	class TCPIOCPServer :
		public NetworkObject
	{
	private:
		PacketMgr m_PacketMgr;
		IOCP m_IOCP;
	public:
		LIBPPNETWORK_API TCPIOCPServer();
		LIBPPNETWORK_API virtual ~TCPIOCPServer();
	public:
		LIBPPNETWORK_API virtual int Run() override;
	public:
		LIBPPNETWORK_API int SetPort(const short* sPort);
		LIBPPNETWORK_API int Startup();
	};
}