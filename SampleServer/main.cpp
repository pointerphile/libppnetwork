#include "../libppnetwork/PPTCPIOCPServer.h"
#include "../libppnetwork/PPTCPIOCPClient.h"
#include "../libppnetwork/PPServerRecvPacketPool.h"
#include "../libppnetwork/PPClientRecvPacketPool.h"
#include "../libppnetwork/PPSender.h"
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")

int ProcessServerPacket();
int ProcessClientPacket();
int StartupServer();
int StartupClient();

int main(int argc, char* argv[]) {
	int iReturn = 0;
	std::locale::global(std::locale(""));

	char charInput;
	std::wcout << L"�۵� ��带 �����Ͻÿ�(1: ����, 2: Ŭ���̾�Ʈ): ";
	std::cin >> charInput;
	std::wcout << std::endl;

	switch (charInput) {
	case '1': {
		//���� ����
		StartupServer();
		break;
	}
	case '2': {
		//Ŭ���̾�Ʈ ����
		StartupClient();
		break;
	}
	default: {
		break;
	}
	}

	return 0;
}

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
		//Ŭ���̾�Ʈ���� ���ڿ� ��Ŷ�� ���Ź��� �� ó���ϴ� �ҽ�
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
		//���ǵ��� ���� ��Ŷ ó�����Դϴ�.
		wcharBuf = (wchar_t*)&packetRecv;
		std::wcout << wcharBuf;
		packetSend = packetRecv;
		pSender->Broadcast(packetSend);
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
		//�������� ���ڿ� ��Ŷ�� ���Ź��� �� ó���ϴ� �ҽ�
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
		//���ǵ��� ���� ��Ŷ ó�����Դϴ�.
		wcharBuf = (wchar_t*)&RecvPacket.m_Packet;
		std::wcout << wcharBuf << std::endl;
		break;
	}
	return 0;
}

int StartupServer() {
	int iReturn = 0;
	std::wcout << L"������ �����մϴ�." << std::endl;
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
		//���� ��ü ���� ó��
		Sleep(2000);
	}
	return 0;
}
int StartupClient() {
	int iReturn = 0;
	std::wcout << L"Ŭ���̾�Ʈ�� �����մϴ�.";
	PP::PPTCPIOCPClient* Client = PP::GetClient();
	PP::PPSender* pSender = PP::GetSender();
	iReturn = Client->SetHost("127.0.0.1");
	iReturn = Client->SetPortNumber(10000);
	iReturn = Client->SetNumberOfThreads(2);
	iReturn = Client->SetFP(ProcessClientPacket);
	iReturn = Client->Startup();

	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}
	while (true) {
		//Ŭ���̾�Ʈ ��ü ���� ó��
		pSender->SendWStringToServer(L"Hello, Server!");
		Sleep(2000);
	}
	return 0;
}