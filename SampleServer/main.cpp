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

int ProcessServerPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv;
	PP::PPPacketForProcess packetSend;
	packetRecv = PP::PPServerRecvPacketPool::GetInstance().front();
	PP::PPServerRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		//클라이언트에게 문자열 패킷을 수신받을 때 처리하는 소스
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
	std::wcout << "injected ProcessClientPacket()..." << std::endl;
	//PPRecvPacketPool에서 저장한 수신패킷을 하나 끄집어내서 처리합니다.
	PP::PPPacketForProcess RecvPacket = PP::PPClientRecvPacketPool::GetInstance().front();
	PP::PPClientRecvPacketPool::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (RecvPacket.m_Packet.m_Header.m_type) {
	case PP::PPPacketType::TYPE_STRING: {
		//서버에게 문자열 패킷을 수신받을 때 처리하는 소스
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
		//정의되지 않은 패킷 처리부입니다.
		wcharBuf = (wchar_t*)&RecvPacket.m_Packet;
		std::wcout << wcharBuf << std::endl;
		break;
	}
	return 0;
}

int StartupServer() {
	int iReturn = 0;
	std::wcout << L"서버를 시작합니다." << std::endl;
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
		//서버 자체 로직 처리
		Sleep(2000);
	}
	return 0;
}
int StartupClient() {
	int iReturn = 0;
	std::wcout << L"클라이언트를 시작합니다.";
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
		//클라이언트 자체 로직 처리
		pSender->SendWStringToServer(L"Hello, Server!");
		Sleep(2000);
	}
	return 0;
}