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
	class WitchOutMatchingServer {
	public:
		PPTCPIOCPServer* m_pServer;
		PPSender* m_pSender;
	public:
		WitchOutMatchingServer();
		~WitchOutMatchingServer();
	public:
		int Init();
		int Run();
		int Release();
	public:
		int GetIPv4Address();
	public:
		static int ProcessPacket();
	};
}
