#include "../libppnetwork/PPTCPIOCPServer.h"							//���� Ŭ���� ����.
#include "../libppnetwork/PPServerRecvPacketPool.h"						//���� ������ �ʿ��մϴ�. �̱��� ��ü.
#include "../libppnetwork/PPTCPIOCPClient.h"							//Ŭ���̾�Ʈ Ŭ���� ����.
#include "../libppnetwork/PPClientRecvPacketPool.h"						//Ŭ���̾�Ʈ ������ �ʿ��մϴ�. �̱��� ��ü
#include "../libppnetwork/PPSender.h"									//���� Ŭ���� ����.
#include "../libppnetwork/PPSessionManager.h"

#include <string>
#pragma comment(lib, "../x64/Debug/libppnetwork.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.

int ProcessServerPacket();
int ProcessClientPacket();
int StartupServer();
int StartupClient();

PP::PPSessionManager* manager = &PP::PPSessionManager::GetInstance();

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

int ProcessServerPacket();
int ProcessClientPacket();

int StartupServer() {
	int iReturn = 0;
	std::wcout << L"������ �����մϴ�." << std::endl;
	PP::PPTCPIOCPServer* Server = PP::GetServer();			//���� ������ü ����
	PP::PPSender* pSender = PP::GetSender();				//���� ��Ŷ���۰�ü ����
	iReturn = Server->SetPortNumber(10000);					//������ ��Ʈ ��ȣ ����
	iReturn = Server->SetNumberOfThreads(2);				//IOCP ������ ����
	iReturn = Server->SetFP(ProcessServerPacket);			//��Ŷ�� ó���� �Լ� ������ ����
	iReturn = Server->Startup();							//���� �õ�
	int iCount = 0;
	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}
	while (true) {
		//���� ��ü ���� ó��
		//pSender->BroadcastWString(L"Hello, Client!");
		if (iCount == 10) {
			break;
		}
		//iCount++;
		Sleep(1000);
	}

	iReturn = Server->Release();							//���� ����
	delete Server;										//���� ������ü ����
	return 0;
}
int StartupClient() {
	int iReturn = 0;
	std::wcout << L"Ŭ���̾�Ʈ�� �����մϴ�.";
	PP::PPTCPIOCPClient* Client = PP::GetClient();			//���� Ŭ���̾�Ʈ��ü ����
	PP::PPSender* pSender = PP::GetSender();				//���� ��Ŷ���۰�ü ����
	iReturn = Client->SetHost("192.168.0.88");					//������ IPv4
	iReturn = Client->SetPortNumber(10000);					//������ ��Ʈ ��ȣ
	iReturn = Client->SetNumberOfThreads(2);				//������ IOCP ������ ����
	iReturn = Client->SetFP(ProcessClientPacket);			//��Ŷ�� ó���� �Լ� ������ ����
	iReturn = Client->Startup();							//Ŭ���̾�Ʈ �õ�
	int iCount = 0;
	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}
	while (true) {
		//Ŭ���̾�Ʈ ��ü ���� ó��
		if (iCount < 10) {
			pSender->SendWStringToServer(L"Hello, Server!");
		}
		if (iCount == 10) {
			for (auto& iter : PP::PPSessionManager::GetInstance().m_mapSession) {
				shutdown(iter.second.m_socketSession, SD_BOTH);
				closesocket(iter.second.m_socketSession);
			}
		}
		iCount++;
		Sleep(1000);
	}
	iReturn = Client->Release();							//���� ����
	delete Client;
	return 0;
}


int ProcessServerPacket() {
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//���� ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	//IOCP �����忡�� �־��� ��Ŷ�� ���� ��ŶǮ ����
	packetRecv = PP::PPServerRecvPacketPool::GetInstance().front();
	//��ŶǮ �� �� pop()
	PP::PPServerRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
		//Ŭ���̾�Ʈ���� ���ڿ� ��Ŷ�� ���Ź��� �� ó���ϴ� �ҽ�
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* ppacketRecvMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		PP::PPPacketMessage packetSendMsg = {};
		int iSizeOfpakcetSendMsg;

		//��Ŷ���� ���� ���ڿ��� ������ �ܼ�â���� ����ϴ� �ҽ�
		wcharBuf = (wchar_t*)&ppacketRecvMsg->m_charMessage;
		wstrBuf = std::to_wstring(packetRecv.m_socketSession);
		wstrBuf.append(L" socket Broadcasting :");
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << packetRecv.m_socketSession << L" socket: " << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());

		//����ü packetSendMsg �ۼ�
		iSizeOfpakcetSendMsg = (int)wstrBuf.size() * 2;
		memcpy(packetSendMsg.m_charMessage, wstrBuf.c_str(), iSizeOfpakcetSendMsg);
		//���� ��Ŷ �ۼ�
		packetSend.m_socketSession = packetRecv.m_socketSession;							//PPPacketForProcess::m_socketSession���� ��Ŷ ������ �Ǵ� �۽��ڸ� ������ �� �ִ�.
		packetSend.m_Mode = PP::PPPacketMode::SEND;											//���� �ƹ� ��ɿ� �������� �ʴ� ������.
		memcpy(packetSend.m_Packet.m_Payload,												//��Ŷ ����� �ۼ�
			(void*)&packetSendMsg, iSizeOfpakcetSendMsg);									//��Ŷ ����ο� memcpy�� ������ ����ü�� deep copy�ؼ� �Է��ϸ� �ȴ�.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfpakcetSendMsg;		//��Ŷ ������� 4����Ʈ + ����� ���̸� ��ģ �� ����
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_STRING;				//��Ŷ Ÿ��. ������ ���� PP::PPPacketType�� ���ǵǾ��ִ�.

		pSender->Broadcast(packetSend);														//PPSessionManager�� �ִ� ��� ���ǵ��� ��ȸ�Ͽ� send�� �ǽ���.
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
	//std::wcout << "injected ProcessClientPacket()..." << std::endl;
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
		std::wcout << wcharBuf;
		OutputDebugStringW(wstrBuf.c_str());
		break;
	}
	case PP::PPPacketType::TYPE_NOTICE_SESSION_EXIT: {
		std::wstring wstrBuf;
		PP::PPPacketNoticeSessionExit* packetRecvNotice = (PP::PPPacketNoticeSessionExit*)RecvPacket.m_Packet.m_Payload;
		wstrBuf.append(L"[����] Ż���� ���ǹ�ȣ(���Ϲ�ȣ): ");
		wstrBuf.append(std::to_wstring(packetRecvNotice->m_socketSession));
		std::wcout << wstrBuf << std::endl;
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