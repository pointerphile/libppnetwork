#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPReceivePacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

void ProcessPacket() {
	if (PPReceivePacketPool::GetInstance().size() != 0) {
		PP_PACKET packet = PPReceivePacketPool::GetInstance().front();
		PPSendPacketPool::GetInstance().push_back(packet);
		PPReceivePacketPool::GetInstance().pop_front();
	}
}

int wmain() {
	PPTCPIOCPServer Server;
	std::thread threadServer;
	//startup
	threadServer = std::thread(&PPTCPIOCPServer::Startup, &Server, 10000, 2);
	threadServer.detach();
	//startup
	while (1) {
		ProcessPacket();
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