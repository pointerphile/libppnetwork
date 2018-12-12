#pragma once
#include "PPNetworkObject.h"
namespace PP {
	class PPSender : public PPNetworkObject {
	public:
		PPSender();
		virtual ~PPSender();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		int Send();
		int Broadcast();
	};
}


