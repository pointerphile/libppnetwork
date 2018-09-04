#include "PPTCPIOCPClient.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPTCPIOCPClient::PPTCPIOCPClient() {}
PPTCPIOCPClient::~PPTCPIOCPClient() {}

int PPTCPIOCPClient::Init() {
	Connector.Init();
	PPIOCP::GetInstance().Init();
	Connector.LaunchThread();

	return 0;
}

int PPTCPIOCPClient::Run() {
	while (!m_isShutdown) {
		if (PPSendPacketPool::GetInstance().size() != 0) {
			Sender.SendFromSendPacketPool();
		}
	}

	return 0;
}

int PPTCPIOCPClient::Release() {
	PPServerObject::m_isShutdown = true;
	PPIOCP::GetInstance().Release();
	Connector.Release();
	PPSessionManager::GetInstance().clear();
	PPSendPacketPool::GetInstance().clear();
	PPReceivePacketPool::GetInstance().clear();

	return 0;
}

int PPTCPIOCPClient::Startup(std::string strAddress, short iPort) {
	PPServerObject::m_isShutdown = false;
	PPIOCP::GetInstance().Set(1);
	Connector.Set(strAddress, iPort);
	if (Init() != 0) {
		return -1;
	}
	Run();

	return 0;
}

int PPTCPIOCPClient::Shutdown() {
	Release();

	return 0;
}
