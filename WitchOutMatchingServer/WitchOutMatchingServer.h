#pragma once
#include "../libppnetwork/PPTCPIOCPServer.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/libppnetwork_d.lib")					//서버 라이브러리의 lib 로드. 실행시 libppnetwork.dll이 반드시 필요합니다.
#else
#pragma comment(lib, "../x64/Release/libppnetwork.lib")					//서버 라이브러리의 lib 로드. 실행시 libppnetwork.dll이 반드시 필요합니다.
#endif // DEBUG

class WitchOutMatchingServer
{
public:
	WitchOutMatchingServer();
	~WitchOutMatchingServer();
};

