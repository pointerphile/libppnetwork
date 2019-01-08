#include "WitchOutMatchingServer.h"
#include "../libppnetwork/PPRecvPacketPoolServer.h"						//���� ������ �ʿ��մϴ�. �̱��� ��ü.

PP::WitchOutMatchingServer::WitchOutMatchingServer() {}
PP::WitchOutMatchingServer::~WitchOutMatchingServer() {}

int PP::WitchOutMatchingServer::Init() {
	m_pServer = GetServer();
	m_pSender = GetSender();
	m_pServer->SetPortNumber(10001);
	m_pServer->SetNumberOfThreads(2);
	m_pServer->SetFP(WitchOutMatchingServer::ProcessPacket);
	return 0;
}

int PP::WitchOutMatchingServer::Run() {
	m_pServer->Startup();
	while (true) {
		Sleep(1000);
	}
	return 0;
}

int PP::WitchOutMatchingServer::Release() {
	m_pServer->Release();
	delete m_pServer;
	delete m_pSender;
	return 0;
}

int PP::WitchOutMatchingServer::ProcessPacket() {
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//���� ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	//IOCP �����忡�� �־��� ��Ŷ�� ���� ��ŶǮ ����
	packetRecv = PP::PPRecvPacketPoolServer::GetInstance().front();
	//��ŶǮ �� �� pop()
	PP::PPRecvPacketPoolServer::GetInstance().pop_front();
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
