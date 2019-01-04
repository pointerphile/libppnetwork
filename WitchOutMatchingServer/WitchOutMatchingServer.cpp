#include "WitchOutMatchingServer.h"

PP::WitchOutMatchingServer::WitchOutMatchingServer() {}
PP::WitchOutMatchingServer::~WitchOutMatchingServer() {}

int PP::WitchOutMatchingServer::Init() {
	
	m_pServer = GetServer();
	m_pSender = GetSender();
	m_pServer->SetPortNumber(10000);
	m_pServer->SetNumberOfThreads(2);
	m_pServer->SetFP(WitchOutMatchingServer::ProcessPacket);
	return 0;
}

int PP::WitchOutMatchingServer::Run() {
	m_pServer->Startup();
	GetIPv4Address();
	while (true) {
		Sleep(1000);
	}
	return 0;
}

int PP::WitchOutMatchingServer::Release() {
	m_pServer->Release();
	return 0;
}

int PP::WitchOutMatchingServer::GetIPv4Address() {
	ULONG iResult = 0;
	ULONG iBufSize = 0;
	PIP_ADAPTER_ADDRESSES pIPv4 = (PIP_ADAPTER_ADDRESSES)new size_t[16384];
	*pIPv4 = {};
	iBufSize = 16384;

	iResult = GetAdaptersAddresses(AF_INET, 0, nullptr, pIPv4, &iBufSize);
	if (iResult != ERROR_SUCCESS) {
		MessageBoxW(nullptr, L"오류: 로컬 IPv4 주소 획득 실패", L"오류", 0);
		return iResult;
	}

	return 0;
}

int PP::WitchOutMatchingServer::ProcessPacket() {
	std::wcout << L"injected" << std::endl;
	return 0;
}
