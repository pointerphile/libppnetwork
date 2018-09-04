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
	int Send();
	//PPSendPacketPool�� �ִ� ��Ŷ�� ���� ���ǵ鿡�� Send�մϴ�.
	int SendFromSendPacketPool();
	int Broadcast();
	//PPSendPacketPool�� �ִ� ��Ŷ�� PPSessionManager�� ���ǵ鿡�� Broadcast�մϴ�.
	int BroadcastFromSendPacketPool();
};

