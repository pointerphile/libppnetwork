#include "PPReceiver.h"

PP::PPReceiver::PPReceiver() {}
PP::PPReceiver::~PPReceiver() {}

int PP::PPReceiver::Init() { return 0; }
int PP::PPReceiver::Run() { return 0; }
int PP::PPReceiver::Release() { return 0; }

int PP::PPReceiver::Recv(PPSession Session, DWORD dwTransferred) {
	bool isReturn = false;
	DWORD dwBytesRead = 0;
	DWORD dwError = 0;
	DWORD dwFlag = 0;

	isReturn = WSARecv(Session.m_socketSession, &Session.m_wsabufRecv, 1, nullptr, &dwFlag, &Session.m_ovRecv, nullptr);
	dwError = WSAGetLastError();
	if (isReturn == true) {
		if (dwBytesRead == 0) {
			if (dwError == WSA_IO_PENDING) {
				//WSA_IO_PENDING
				DisplayError(L"WSARecv()");
				return 0;
			}
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WSARecv()");
				return -1;
			}
		}
		else {
			PPRecvPacket packetRecv;
			packetRecv.m_socketSession = Session.m_socketSession;
			memcpy((void*)&packetRecv, Session.m_wsabufRecv.buf, dwTransferred);
			//PPReceivePacketPool::GetInstance();
		}
	}
	else {
		if (dwError == WSA_IO_PENDING) {
			DisplayError(L"WSARecv()");
			return 0;
		}
		if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
			DisplayError(L"WSARecv()");
			return -1;
		}
	}
	return 0;
}