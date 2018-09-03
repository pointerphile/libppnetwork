#include "PPIOCPTCPServer.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPIOCPTCPServer::PPIOCPTCPServer() {}
PPIOCPTCPServer::~PPIOCPTCPServer() {}

int PPIOCPTCPServer::Init() {
	Acceptor.Init();
	PPIOCP::GetInstance().Init();
	Acceptor.LaunchThread();

	return 0;
}

int PPIOCPTCPServer::Run() {
	while (!m_isShutdown) {
		if (PPSendPacketPool::GetInstance().size() != 0) {
			Sender.Broadcast();
		}
	}

	return 0;
}

int PPIOCPTCPServer::Release() {
	PPServerObject::m_isShutdown = true;
	PPIOCP::GetInstance().Release();
	Acceptor.Release();
	PPSessionManager::GetInstance().clear();
	PPSendPacketPool::GetInstance().clear();
	PPReceivePacketPool::GetInstance().clear();

	return 0;
}

int PPIOCPTCPServer::Startup(short iPort, short iThread) {
	PPServerObject::m_isShutdown = false;
	PPIOCP::GetInstance().Set(iThread);
	Acceptor.Set(iPort);
	if (Init() != 0) {
		return -1;
	}
	Run();

	return 0;
}

int PPIOCPTCPServer::Shutdown() {
	Release();

	return 0;
}