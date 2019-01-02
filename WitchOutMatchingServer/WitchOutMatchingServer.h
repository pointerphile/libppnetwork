#pragma once
#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPRecvPacketPoolServer.h"
#include "../libppnetwork/PPSender.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/libppnetwork_d.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#else
#pragma comment(lib, "../x64/Release/libppnetwork.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#endif // DEBUG

namespace PP {
	class WitchOutMatchingServer : public PPNetworkObject {
	public:
		PPTCPIOCPServer* m_pServer;
		PPSender* m_pSender;
	public:
		WitchOutMatchingServer();
		~WitchOutMatchingServer();
	public:
		virtual int Init();
		virtual int Run();
		virtual int Release();
	public:
		int ProcessPacket();
	};
}
