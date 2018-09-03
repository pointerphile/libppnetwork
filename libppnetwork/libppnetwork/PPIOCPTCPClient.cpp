#include "PPIOCPTCPClient.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPIOCPTCPClient::PPIOCPTCPClient() {}


PPIOCPTCPClient::~PPIOCPTCPClient() {}

int PPIOCPTCPClient::Init()
{
	return 0;
}

int PPIOCPTCPClient::Run()
{
	return 0;
}

int PPIOCPTCPClient::Release()
{
	return 0;
}

int PPIOCPTCPClient::Startup(std::string strAddress, short iPort)
{
	PPServerObject::m_isShutdown = false;
	PPIOCP::GetInstance().Set(1);
	Connector.Set(strAddress, iPort);
	if (Init() != 0) {
		return -1;
	}
	Run();

	return 0;
	return 0;
}

int PPIOCPTCPClient::Shutdown()
{
	return 0;
}
