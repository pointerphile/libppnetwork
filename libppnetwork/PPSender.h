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
	private:
		int Send(PPSession Session, DWORD dwBytesToWrite);
		int SendRawString(PPSession Session, std::wstring wstrMessage);
		int Broadcast(PPSession Session, DWORD dwBytesToWrite);
	public:
		LIBPPNETWORK_API int SendWStringToServer(std::wstring wstrMessage);
		LIBPPNETWORK_API int Broadcast(PPPacketForProcess packetSend);
		LIBPPNETWORK_API int BroadcastWString(std::wstring wstrMessage);
		LIBPPNETWORK_API int BroadcastRawWString(std::wstring wstrMessage);
	};
	LIBPPNETWORK_API PP::PPSender* GetSender();
}

