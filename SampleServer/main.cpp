#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPSender.h"
#include "../libppnetwork/PPRecvPacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int inject() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	std::wcout << "injected function()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess RecvPacket = PP::PPRecvPacketPool::GetInstance().front();
	PP::PPPacketForProcess packetSend;
	PP::PPRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)RecvPacket.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());
		break;
	}
	case PP::TYPE_MOVE_HOST_TO_GUEST: {
		PP::PPPacketStartMoveObjectGuestToHost *packetMoveRecv = (PP::PPPacketStartMoveObjectGuestToHost*)RecvPacket.m_Packet.m_Payload;
		PP::PPPacketStartMoveObjectGuestToHost packetMoveSend;
		packetMoveSend.m_iObjectID = packetMoveRecv->m_iObjectID;
		packetMoveSend.m_fNormalx = packetMoveRecv->m_fNormalx;
		packetMoveSend.m_fNormaly = packetMoveRecv->m_fNormaly;
		packetMoveSend.m_fNormalz = packetMoveRecv->m_fNormalz;
		packetMoveSend.m_fSpeed = packetMoveRecv->m_fSpeed;
		memcpy(packetSend.m_Packet.m_Payload, &packetMoveSend, sizeof(packetMoveSend));
		packetSend.m_Packet.m_Header.m_type = PP::TYPE_MOVE_GUEST_TO_HOST;
		packetSend.m_Packet.m_Header.m_len = sizeof(packetMoveSend) + PACKET_HEADER_SIZE;
		pSender->Broadcast(packetSend);
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
		Sleep(2000);
	}

	return 0;
}