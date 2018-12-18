#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPSender.h"
#include "../libppnetwork/PPRecvPacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int inject() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	std::wcout << "injected function()..." << std::endl;
	PP::PPRecvPacket RecvPacket = PP::PPRecvPacketPool::GetInstance().m_listRecvPacket.front();
	PP::PPRecvPacketPool::GetInstance().m_listRecvPacket.pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)RecvPacket.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		std::wcout << wcharBuf << std::endl;
		break;
	}
	default:
		wcharBuf = (wchar_t*)&RecvPacket;
		std::wcout << wcharBuf;
		break;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	std::locale::global(std::locale(""));

	int iReturn = 0;
	PP::PPTCPIOCPServer* Server = PP::GetServer();
	PP::PPSender* pSender = PP::GetSender();
	iReturn = Server->SetPortNumber(10000);
	iReturn = Server->SetNumberOfThreads(2);
	iReturn = Server->SetFP(inject);
	iReturn = Server->Startup();
	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}

	while (true) {
		PP::PPSendPacket packetSend0;
		packetSend0.m_SendMode = PP::PPSendMode::BROADCAST;
		packetSend0.m_socketSession = 0;
		
		PP::PPPacketMove packetMove0 { -1.0f, 0.0f, 0.0f, 2.0f };
		memcpy(packetSend0.m_Packet.m_Payload, &packetMove0, sizeof(packetMove0));
		packetSend0.m_Packet.m_Header.m_type = PP::TYPE_MOVE;
		packetSend0.m_Packet.m_Header.m_len = sizeof(packetMove0) + PACKET_HEADER_SIZE;
		pSender->Broadcast(packetSend0);
		//pSender->BroadcastRawWString(L"Hello, Client!\0");
		Sleep(2000);

		PP::PPSendPacket packetSend1;
		packetSend1.m_SendMode = PP::PPSendMode::BROADCAST;
		packetSend1.m_socketSession = 0;

		PP::PPPacketMove packetMove1{ 1.0f, 0.0f, 0.0f, 2.0f };
		memcpy(packetSend1.m_Packet.m_Payload, &packetMove1, sizeof(packetMove1));
		packetSend1.m_Packet.m_Header.m_type = PP::TYPE_MOVE;
		packetSend1.m_Packet.m_Header.m_len = sizeof(packetMove1) + PACKET_HEADER_SIZE;
		pSender->Broadcast(packetSend1);
		Sleep(2000);
	}

	return 0;
}