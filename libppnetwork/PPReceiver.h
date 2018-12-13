#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPReceiver : public PPNetworkObject {
	public:
		PPReceiver();
		virtual ~PPReceiver();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		int Recv(PPSession Session, DWORD dwTransferred);
	};
}


