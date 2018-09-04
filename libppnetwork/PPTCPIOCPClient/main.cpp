#include "../libppnetwork/PPTCPIOCPClient.h"
#include "../libppnetwork/PPSessionManager.h"
#include "../libppnetwork/PPReceivePacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int ProcessPacket() {
	PPSender Sender;
	if (PPReceivePacketPool::GetInstance().size() != 0) {
		PP_PACKET packet = PPReceivePacketPool::GetInstance().front();
		if (strlen(packet.m_packet.m_msg) != 0) {
			std::cout << packet.m_packet.m_msg << std::endl;
		}
	}
	if (PPSendPacketPool::GetInstance().size() != 0) {
		Sender.SendFromSendPacketPool();
	}
	return 0;
}

int _tmain() {
	PPTCPIOCPClient Client;
	std::thread threadClient;
	//startup
	threadClient = std::thread(&PPTCPIOCPClient::Startup, &Client, "127.0.0.1", 10000);
	threadClient.detach();
	//startup
	while (1) {
		PP_PACKET packet = { 0 };
		packet.m_pSession;// = PPSessionManager::GetInstance()
		ProcessPacket();
		if (GetAsyncKeyState(VK_F12) & 0x8000)
		{
			//shutdown
			Client.Shutdown();
			//shutdown
			break;
		}
	}

	return 0;
}