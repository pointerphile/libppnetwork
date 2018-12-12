#include "../libppnetwork/PPTCPIOCPServer.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int main(int argc, char* argv[]) {
	int iReturn = 0;
	PP::PPTCPIOCPServer* Server = PP::GetServer();
	iReturn = Server->Init();
	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}

	while (true) {
		OutputDebugStringW(L"main()\n");
		Sleep(1000);
	}

	return 0;
}