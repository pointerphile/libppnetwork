#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPSender.h"
#include "../libppnetwork/PPRecvPacketPool.h"
#include "../libppnetwork/PPSendPacketPool.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int inject() {
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//���� ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
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
		pSender->BroadcastRawWString(L"Hello, Client!\0");
		Sleep(1000);
	}

	return 0;
}