#pragma once
#include "../libppnetwork/PPTCPIOCPServer.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/libppnetwork_d.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#else
#pragma comment(lib, "../x64/Release/libppnetwork.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#endif // DEBUG

class WitchOutMatchingServer
{
public:
	WitchOutMatchingServer();
	~WitchOutMatchingServer();
};

