#pragma once
#include "ppnetworkobject.h"
namespace PP {
	class LIBPPNETWORK_API PPTCPIOCPServer : public PPNetworkObject {
	public:
		PPTCPIOCPServer();
		virtual ~PPTCPIOCPServer();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	};
}


