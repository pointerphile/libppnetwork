#include "PPReceiver.h"

PP::PPReceiver::PPReceiver() {}
PP::PPReceiver::~PPReceiver() {}

int PP::PPReceiver::Init() { return 0; }
int PP::PPReceiver::Run() { return 0; }
int PP::PPReceiver::Release() { return 0; }

int PP::PPReceiver::Recv(PPSession Session, DWORD dwTransferred) {
	int iReturn = 0;
	DWORD dwBytesRead = 0;
	DWORD dwError = 0;
	DWORD dwFlag = 0;

	iReturn = WSARecv(Session.m_socketSession, &Session.m_wsabufRecv, 1, nullptr, &dwFlag, &Session.m_ovRecv, nullptr);
	if (iReturn == SOCKET_ERROR) {
		dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING) {
			DisplayError(L"WSARecv()");
			return -1;
		}
	}
	return 0;
}