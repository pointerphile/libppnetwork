#include "../libppnetwork/PPTCPIOCPServer.h"							//서버 클래스 정의.
#include "../libppnetwork/PPRecvPacketPoolServer.h"						//서버 구동시 필요합니다. 싱글톤 객체.
#include "../libppnetwork/PPTCPIOCPClient.h"							//클라이언트 클래스 정의.
#include "../libppnetwork/PPRecvPacketPoolClient.h"						//클라이언트 구동시 필요합니다. 싱글톤 객체
#include "../libppnetwork/PPSender.h"									//센더 클래스 정의.
#include "../libppnetwork/PPSessionManager.h"
#include <fstream>

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/libppnetwork_d.lib")					//서버 라이브러리의 lib 로드. 실행시 libppnetwork.dll이 반드시 필요합니다.
#else
#pragma comment(lib, "../x64/Release/libppnetwork.lib")					//서버 라이브러리의 lib 로드. 실행시 libppnetwork.dll이 반드시 필요합니다.
#endif // DEBUG


int ProcessServerPacket();
int ProcessClientPacket();
int StartupServer();
int StartupClient();

int main(int argc, char* argv[]) {
	int iReturn = 0;
	std::locale::global(std::locale(""));

	char charInput;
	std::wcout << L"작동 모드를 선택하시오(1: 서버, 2: 클라이언트): ";
	std::cin >> charInput;
	std::wcout << std::endl;

	switch (charInput) {
	case '1': {
		//서버 시작
		StartupServer();
		break;
	}
	case '2': {
		//클라이언트 시작
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

	std::wcout << L"서버를 시작합니다." << std::endl;
	PP::PPTCPIOCPServer* Server = PP::GetServer();			//동적 서버객체 생성
	PP::PPSender* pSender = PP::GetSender();				//동적 패킷전송객체 생성
	iReturn = Server->SetPortNumber(10000);					//서버의 포트 번호 지정
	iReturn = Server->SetNumberOfThreads(2);				//IOCP 스레드 개수
	iReturn = Server->SetFP(ProcessServerPacket);			//패킷을 처리할 함수 포인터 지정
	iReturn = Server->Startup();							//서버 시동
	int iCount = 0;
	if (iReturn != 0) {
		std::system("pause");
		return iReturn;
	}
	while (true) {
		//서버 자체 로직 처리
		//pSender->BroadcastWString(L"Hello, Client!");
		Sleep(1000);
	}

	iReturn = Server->Release();							//서버 종료
	delete Server;											//동적 서버객체 삭제
	return 0;
}
int StartupClient() {
	int iReturn = 0;
	std::string strIPv4;
	std::ifstream ifIPv4;
	ifIPv4.open("IPv4.txt");
	ifIPv4 >> strIPv4;
	ifIPv4.close();

	std::wcout << L"클라이언트를 시작합니다.";
	PP::PPTCPIOCPClient* Client = PP::GetClient();			//동적 클라이언트객체 생성
	PP::PPSender* pSender = PP::GetSender();				//동적 패킷전송객체 생성

	iReturn = Client->SetHost(strIPv4.c_str());				//서버의 IPv4
	iReturn = Client->SetPortNumber(10000);					//서버의 포트 번호
	iReturn = Client->SetNumberOfThreads(2);				//생성할 IOCP 스레드 개수
	iReturn = Client->SetFP(ProcessClientPacket);			//패킷을 처리할 함수 포인터 지정
	iReturn = Client->Startup();							//클라이언트 시동

	int iCount = 0;
	if (iReturn != 0) {
		std::wcout << L"실패" << iReturn << std::endl;
		std::system("pause");
		return iReturn;
	}
	while (true) {
		//클라이언트 자체 로직 처리
		
		pSender->SendWStringToServer(L"Hello, Server!");
		Sleep(1000);
	}
	iReturn = Client->Release();
	delete Client;
	return 0;
}


int ProcessServerPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	//IOCP 스레드에서 넣엇던 패킷을 담은 패킷풀 접근
	packetRecv = PP::PPRecvPacketPoolServer::GetInstance().front();
	//패킷풀 맨 앞 pop()
	PP::PPRecvPacketPoolServer::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
		//클라이언트에게 문자열 패킷을 수신받을 때 처리하는 소스
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* ppacketRecvMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		PP::PPPacketMessage packetSendMsg = {};
		int iSizeOfpakcetSendMsg;

		//패킷에서 꺼낸 문자열을 서버의 콘솔창으로 출력하는 소스
		wcharBuf = (wchar_t*)&ppacketRecvMsg->m_charMessage;
		wstrBuf = std::to_wstring(packetRecv.m_socketSession);
		wstrBuf.append(L" socket Broadcasting :");
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << packetRecv.m_socketSession << L" socket: " << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());

		//구조체 packetSendMsg 작성
		iSizeOfpakcetSendMsg = (int)wstrBuf.size() * 2;
		memcpy(packetSendMsg.m_charMessage, wstrBuf.c_str(), iSizeOfpakcetSendMsg);
		//보낼 패킷 작성
		packetSend.m_socketSession = packetRecv.m_socketSession;							//PPPacketForProcess::m_socketSession으로 패킷 수신자 또는 송신자를 지정할 수 있다.
		packetSend.m_Mode = PP::PPPacketMode::SEND;											//수신받은 패킷인지 송신할 패킷인지 지정
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSendMsg, iSizeOfpakcetSendMsg);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfpakcetSendMsg;		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_STRING;				//패킷 타입. 열거형 변수 PP::PPPacketType에 정의되어있다.

		pSender->Broadcast(packetSend);														//PPSessionManager에 있는 모든 세션들을 순회하여 send를 실시함.
		break;
	}
	case PP::PPPacketType::TYPE_REQ_SOCKET: {
		PP::PPPacketNoticeSession packetSession = {};
		int iSizeOfPayload = sizeof(packetSession);
		packetSession.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_Mode = PP::PPPacketMode::SEND;
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSession, iSizeOfPayload);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfPayload;		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_ACK_SOCKET;

		pSender->Send(packetSend);
		break;
	}

	default:
		//정의되지 않은 패킷 처리부입니다.
		wcharBuf = (wchar_t*)&packetRecv;
		std::wcout << wcharBuf;
		packetSend = packetRecv;
		pSender->Broadcast(packetSend);
		break;
	}
	return 0;
}

int ProcessClientPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//클라이언트 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessClientPacket()..." << std::endl;
	//PPRecvPacketPool에서 저장한 수신패킷을 하나 끄집어내서 처리합니다.
	PP::PPPacketForProcess RecvPacket = PP::PPRecvPacketPoolClient::GetInstance().front();
	PP::PPRecvPacketPoolClient::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		//서버에게 문자열 패킷을 수신받을 때 처리하는 소스
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
		wstrBuf.append(L"[공지] 탈주한 세션번호(소켓번호): ");
		wstrBuf.append(std::to_wstring(packetRecvNotice->m_socketSession));
		std::wcout << wstrBuf << std::endl;
		break;
	}
	case PP::PPPacketType::TYPE_ACK_SOCKET: {
		PP::PPPacketNoticeSession* packetSession = (PP::PPPacketNoticeSession*)RecvPacket.m_Packet.m_Payload;
		std::wcout << packetSession->m_socketSession << std::endl;
		break;
	}
	default:
		//정의되지 않은 패킷 처리부입니다.
		wcharBuf = (wchar_t*)&RecvPacket.m_Packet;
		std::wcout << wcharBuf << std::endl;
		break;
	}
	return 0;
}