#include "../libppnetwork/PPTCPIOCPServer.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int main(int argc, char* argv[]) {
	PP::PPTCPIOCPServer* Server = PP::GetServer();
	Server->LaunchThread();
	while (true) {
		OutputDebugStringW(L"main()�ƾƾ�\n");
		Sleep(1000);
	}

	return 0;
}