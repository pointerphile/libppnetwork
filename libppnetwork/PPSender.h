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
		LIBPPNETWORK_API int Send(PPPacketForProcess packetSend);
		LIBPPNETWORK_API int SendToServer(PPPacketForProcess packetSend);
		LIBPPNETWORK_API int SendWStringToServer(std::wstring wstrMessage);
		LIBPPNETWORK_API int Broadcast(PPPacketForProcess packetSend);
		LIBPPNETWORK_API int BroadcastExcept(PPPacketForProcess packetSend);
		LIBPPNETWORK_API int BroadcastWString(std::wstring wstrMessage);
		LIBPPNETWORK_API int BroadcastRawWString(std::wstring wstrMessage);
	};
	LIBPPNETWORK_API PP::PPSender* GetSender();
}

