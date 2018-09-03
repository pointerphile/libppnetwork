#include "PPReceiver.h"
#include "PPReceivePacketPool.h"

PPReceiver::PPReceiver() {}
PPReceiver::~PPReceiver() {}

int PPReceiver::Init() { return 0; }
int PPReceiver::Run() { return 0; }
int PPReceiver::Release() { return 0; }

int PPReceiver::Receive(PPSession* pSession, DWORD dwTransferred)
{
	if (pSession->m_socketSession == INVALID_SOCKET) {
		DisplayError(_TEXT("PPReceiver.Run()"));
		return -1;
	}
	int iResult = 0;
	DWORD flags = 0;
	pSession->m_ov = { 0 };
	pSession->m_wsabufRecv.buf = pSession->m_bufRecv;
	pSession->m_wsabufRecv.len = 2048;
	iResult = WSARecv(
		pSession->m_socketSession,
		&pSession->m_wsabufRecv,
		1,
		nullptr,
		&flags,
		&pSession->m_ov,
		nullptr);
	if (iResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DisplayError(_TEXT("WSARecv()"));
			return -1;
		}
	}
	if (pSession->m_bufRecv != '\0') {
		//std::cout << "socket " << pSession->m_socketSession
		//	<< " Received : " << pSession->m_bufRecv << std::endl;
		UPACKET packet = { 0 };
		if (strlen(pSession->m_bufRecv) > 0) {
			memcpy(packet.m_msg, pSession->m_bufRecv, strlen(pSession->m_bufRecv));
		}
		else {
			return -1;
		}
		PPReceivePacketPool::GetInstance().push_back(packet, pSession);
		pSession->m_wsabufRecv = { 0 };
		ZeroMemory(pSession->m_bufRecv, sizeof(pSession->m_bufRecv));
	}
	return 0;
}