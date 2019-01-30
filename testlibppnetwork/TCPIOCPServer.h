#pragma once
#include "NetworkObject.h"
#include "IOCP.h"

namespace PP{
	class TCPIOCPServer :
		public NetworkObject
	{
	private:
		bool bEnable;
		SOCKET m_socketListen;
		sockaddr_in m_saListen;
		unsigned short m_usPortNumber;
		PacketMgr m_PacketMgr;
		IOCP m_IOCP;
	public:
		LIBPPNETWORK_API TCPIOCPServer();
		LIBPPNETWORK_API virtual ~TCPIOCPServer();
	public:
		virtual int Init() override;
		virtual int Run() override;
		virtual int Release() override;
	public:
		LIBPPNETWORK_API int SetPort(const short& usPortNumber);
		LIBPPNETWORK_API int Startup();
	};
}