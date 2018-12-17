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
		int Send(PPSession Session, DWORD dwBytesToWrite);
		int Broadcast(PPSession Session, DWORD dwBytesToWrite);
		int Broadcast(PPSendPacket packetSend);
		int BroadcastWString(std::wstring wstrMessage);
		int BroadcastRawString(std::wstring wstrMessage);
	};
}


