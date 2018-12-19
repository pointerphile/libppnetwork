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
		int SendRawString(PPSession Session, std::wstring wstrMessage);
		int Broadcast(PPSession Session, DWORD dwBytesToWrite);
		LIBPPNETWORK_API int Broadcast(PPSendPacket packetSend);
		LIBPPNETWORK_API int BroadcastWString(std::wstring wstrMessage);
		LIBPPNETWORK_API int BroadcastRawWString(std::wstring wstrMessage);
	};
	LIBPPNETWORK_API PP::PPSender* GetSender();
}

