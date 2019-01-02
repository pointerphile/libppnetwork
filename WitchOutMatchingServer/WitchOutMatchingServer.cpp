#include "WitchOutMatchingServer.h"

PP::WitchOutMatchingServer::WitchOutMatchingServer() {}
PP::WitchOutMatchingServer::~WitchOutMatchingServer() {}

int PP::WitchOutMatchingServer::Init() {
	m_pServer = GetServer();
	m_pSender = GetSender();
	m_pServer->SetPortNumber(10000);
	m_pServer->SetNumberOfThreads(2);
	m_pServer->SetFP(this->ProcessPacket);
	return 0;
}

int PP::WitchOutMatchingServer::Run() {
	return 0;
}

int PP::WitchOutMatchingServer::Release() {
	return 0;
}

int PP::WitchOutMatchingServer::ProcessPacket() {
	return 0;
}
