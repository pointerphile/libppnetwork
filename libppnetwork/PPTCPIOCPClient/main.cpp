#include "../libppnetwork/PPTCPIOCPClient.h"
#include "../libppnetwork/PPSessionManager.h"
#include "../libppnetwork/PPReceivePacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

bool isGameRun = false;

int ProcessPacket() {
	PPSender Sender;
	while (isGameRun) {
		if (PPReceivePacketPool::GetInstance().size() != 0) {
			PP_PACKET packet = PPReceivePacketPool::GetInstance().front();
			if (strlen(packet.m_packet.m_msg) != 0) {
				std::cout << packet.m_packet.m_msg << std::endl;
			}
		}
		if (PPSendPacketPool::GetInstance().size() != 0) {
			Sender.SendFromSendPacketPool();
		}
	}
	return 0;
}

int ProcessGame() {
	PP_PACKET packet = {};
	return 0;
}

int _tmain() {
	PPTCPIOCPClient Client;
	std::thread threadClient;
	std::thread threadPacketProcessor;

	isGameRun = true;

	//startup
	threadClient = std::thread(&PPTCPIOCPClient::Startup, &Client, "127.0.0.1", 10000);
	threadClient.detach();
	//startup
	threadPacketProcessor = std::thread(ProcessPacket);
	threadPacketProcessor.detach();

	while (isGameRun) {
		ProcessGame();
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