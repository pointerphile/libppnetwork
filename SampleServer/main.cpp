#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPTCPIOCPClient.h"
#include "../libppnetwork/PPServerRecvPacketPool.h"
#include "../libppnetwork/PPClientRecvPacketPool.h"
#include "../libppnetwork/PPSender.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int ProcessServerPacket() {
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//���� ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
	std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	packetRecv = PP::PPServerRecvPacketPool::GetInstance().front();
	PP::PPServerRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* packetMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		wcharBuf = (wchar_t*)&packetMsg->m_charMessage;
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());
		pSender->BroadcastWString(wstrBuf.c_str());
		break;
	}
	default:
		wcharBuf = (wchar_t*)&packetRecv;
		std::wcout << wcharBuf;
		break;
	}
	return 0;
}

int ProcessClientPacket() {
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//Ŭ���̾�Ʈ ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
	std::wcout << "injected ProcessClientPacket()..." << std::endl;
	//PPRecvPacketPool���� ������ ������Ŷ�� �ϳ� ������� ó���մϴ�.
	PP::PPPacketForProcess RecvPacket = PP::PPClientRecvPacketPool::GetInstance().front();
	PP::PPClientRecvPacketPool::GetInstance().pop_front();
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
	default:
		wcharBuf = (wchar_t*)&RecvPacket.m_Packet;
		std::wcout << wcharBuf << std::endl;
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
	iReturn = Server->SetFP(ProcessServerPacket);
	iReturn = Server->Startup();

	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}
	while (true) {
		pSender->SendWStringToServer(L"Hello, Client!");
		Sleep(2000);
	}

	return 0;
}