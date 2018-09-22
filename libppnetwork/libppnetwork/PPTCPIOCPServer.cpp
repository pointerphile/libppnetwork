#include "PPTCPIOCPServer.h"
#include "PPSessionManager.h"
#include "PPReceivePacketPool.h"
#include "PPSendPacketPool.h"
#include "PPIOCP.h"

PPTCPIOCPServer::PPTCPIOCPServer() {}
PPTCPIOCPServer::~PPTCPIOCPServer() {}

int PPTCPIOCPServer::Init() {
	PPReceivePacketPool::GetInstance().Init();
	PPSendPacketPool::GetInstance().Init();
	Acceptor.Init();
	PPIOCP::GetInstance().Init();
	Acceptor.LaunchThread();

	return 0;
}

int PPTCPIOCPServer::Run() { return 0; }

int PPTCPIOCPServer::Release() {
	std::cout << "PPTCPIOCPServer::Release()" << std::endl;
	PPServerObject::m_isShutdown = true;
	PPIOCP::GetInstance().Release();
	Acceptor.Release();
	PPSessionManager::GetInstance().clear();
	PPReceivePacketPool::GetInstance().Release();
	PPSendPacketPool::GetInstance().Release();

	return 0;
}

int PPTCPIOCPServer::Startup(short iPort, short iThread) {
	PPServerObject::m_isShutdown = false;
	PPIOCP::GetInstance().Set(iThread);
	Acceptor.Set(iPort);
	if (this->Init() != 0) {
		return -1;
	}
	Run();

	return 0;
}

int PPTCPIOCPServer::Shutdown() {
	Release();

	return 0;
}