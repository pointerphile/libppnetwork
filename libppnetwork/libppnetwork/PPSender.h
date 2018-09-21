#pragma once
#include "PPServerObject.h"
class PPSender : public PPServerObject {
public:
	PPSender();
	virtual ~PPSender();
public:
	int Init();
	int Run();
	int Release();
public:
	int Send(PPSession Session, DWORD dwBytesToWrite);
	//PPSendPacketPool에 있는 패킷을 보낸 세션들에게 Send합니다.
	int SendFromSendPacketPool();
	int Broadcast(PPSession Session, DWORD dwBytesToWrite);
	//PPSendPacketPool에 있는 패킷을 PPSessionManager의 세션들에게 Broadcast합니다.
	int BroadcastFromSendPacketPool();
};

