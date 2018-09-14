#include "PPReceiver.h"
#include "PPReceivePacketPool.h"

PPReceiver::PPReceiver() {}
PPReceiver::~PPReceiver() {}

int PPReceiver::Init() { return 0; }
int PPReceiver::Run() { return 0; }
int PPReceiver::Release() { return 0; }

int PPReceiver::Receive(PPSession Session, DWORD dwTransferred)
{
	bool isReturn = false;
	DWORD dwBytesRead = 0;
	DWORD dwError = 0;
	DWORD dwFlag = 0;

	//isReturn = ReadFile(m_hFileRead, m_bufData, dwBytesToRead, &dwBytesRead, (LPOVERLAPPED)&m_ovRead);
	isReturn = WSARecv(Session.m_socketSession, &Session.m_wsabufRecv, 1, &dwBytesRead, &dwFlag, &Session.m_ovRecv, nullptr);
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