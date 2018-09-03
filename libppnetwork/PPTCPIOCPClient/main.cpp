#include "../libppnetwork/PPTCPIOCPClient.h"
#include "../libppnetwork/PPReceivePacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

void process() {
}

int main() {
	PPTCPIOCPClient Client;
	std::thread threadClient;
	//startup
	threadClient = std::thread(&PPTCPIOCPClient::Startup, &Client, "127.0.0.1", 10000);
	threadClient.detach();
	//startup
	while (1) {
		process();
		if (GetAsyncKeyState(VK_F12) & 0x8000)
		{
			//shutdown
			Client.Shutdown();
			//shutdown
			break;
		}
	}

	Sleep(1000);
	system("pause");

	threadClient = std::thread(&PPTCPIOCPClient::Startup, &Client, "127.0.0.1", 10000);
	threadClient.detach();
	while (1) {
		process();
		if (GetAsyncKeyState(VK_F12) & 0x8000)
		{
			Client.Shutdown();
			break;
		}
	}

	return 0;
}