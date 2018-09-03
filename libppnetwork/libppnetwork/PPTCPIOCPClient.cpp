#include "PPTCPIOCPClient.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPTCPIOCPClient::PPTCPIOCPClient() {}
PPTCPIOCPClient::~PPTCPIOCPClient() {}

int PPTCPIOCPClient::Init() {
	return 0;
}

int PPTCPIOCPClient::Run() {
	return 0;
}

int PPTCPIOCPClient::Release() {
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
	return 0;
}
