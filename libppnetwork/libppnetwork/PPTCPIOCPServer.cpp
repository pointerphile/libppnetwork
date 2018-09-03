#include "PPTCPIOCPServer.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPTCPIOCPServer::PPTCPIOCPServer() {}
PPTCPIOCPServer::~PPTCPIOCPServer() {}

int PPTCPIOCPServer::Init() {
	Acceptor.Init();
	PPIOCP::GetInstance().Init();
	Acceptor.LaunchThread();

	return 0;
}

int PPTCPIOCPServer::Run() {
	while (!m_isShutdown) {
		if (PPSendPacketPool::GetInstance().size() != 0) {
			Sender.Broadcast();
		}
	}

	return 0;
}

int PPTCPIOCPServer::Release() {
	PPServerObject::m_isShutdown = true;
	PPIOCP::GetInstance().Release();
	Acceptor.Release();
	PPSessionManager::GetInstance().clear();
	PPSendPacketPool::GetInstance().clear();
	PPReceivePacketPool::GetInstance().clear();

	return 0;
}

int PPTCPIOCPServer::Startup(short iPort, short iThread) {
	PPServerObject::m_isShutdown = false;
	PPIOCP::GetInstance().Set(iThread);
	Acceptor.Set(iPort);
	if (Init() != 0) {
		return -1;
	}
	Run();

	return 0;
}

int PPTCPIOCPServer::Shutdown() {
	Release();

	return 0;
}