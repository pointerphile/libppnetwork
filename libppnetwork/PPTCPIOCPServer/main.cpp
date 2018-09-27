#include "../libppnetwork/PPTCPIOCPServer.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int main() {
	PPTCPIOCPServer Server;
	std::thread threadServer;
	//startup
	threadServer = std::thread(&PPTCPIOCPServer::Startup, &Server, 10000, 2);
	threadServer.detach();
	//startup
	while (1) {
		if (GetAsyncKeyState(VK_F12) & 0x8000)
		{
			//shutdown
			Server.Shutdown();
			//shutdown
			break;
		}
	}

	return 0;
}