#include "PPIOCPWorker.h"
#include "PPIOCP.h"
#include "PPSessionManager.h"

PPIOCPWorker::PPIOCPWorker() {}
PPIOCPWorker::~PPIOCPWorker() {}

int PPIOCPWorker::Init() {
	return 0;
}

int PPIOCPWorker::Run() {
	std::cout << "PPIOCPWorker::Run()" << std::endl;
	bool isReturn = true;
	DWORD dwTransferred = 0;
	unsigned __int64 lpCompletionKey;
	PPOVERLAPPED* overlapped;
	while (1) {
		lpCompletionKey = 0;
		overlapped = 0;
		//if (WaitForSingleObject(m_hEvent, 1) == WAIT_OBJECT_0) {
		//	break;
		//}

		isReturn = GetQueuedCompletionStatus(PPIOCP::GetInstance().m_hIOCP, &dwTransferred, &lpCompletionKey, (LPOVERLAPPED*)&overlapped, INFINITE);
		std::map<SOCKET, PPSession>::iterator iter = PPSessionManager::GetInstance().find((SOCKET)lpCompletionKey);

		if (isReturn == true) {
			if (dwTransferred != 0) {
				if (lpCompletionKey != 0 && overlapped != 0) {
					switch (overlapped->dwFlag) {
					case ASYNCFLAG_RECV:
						DispatchRecv(iter->second, dwTransferred);
						break;
					case ASYNCFLAG_SEND:
						DispatchSend(iter->second, dwTransferred);
						break;
					}
				}
			}
			else {
				//dwTransferred == 0 : 소켓 닫힘
				DisplayError(L"GetQueuedCompletionStatus");
				PPSessionManager::GetInstance().erase(iter->first);
				continue;
			}
		}
		else {
			DWORD dwError = WSAGetLastError();
			if (dwError == WSAETIMEDOUT) {
				continue;
			}
			else {
				if (overlapped != nullptr) {
					//클라이언트 비정상종료
					DisplayError(L"GetQueuedCompletionStatus()");
					PPSessionManager::GetInstance().erase(iter->first);
				}
				else {
					DisplayError(L"GetQueuedCompletionStatus()");
					exit(-1);
				}
			}
		}
	}
}

int PPIOCPWorker::Release() { return 0; }

int PPIOCPWorker::DispatchRecv(PPSession Session, DWORD dwTransferred)
{
	int iReturn = 0;
	LARGE_INTEGER lr;
	lr.QuadPart = dwTransferred;
	Session.m_ovRecv.Offset += lr.LowPart;
	Session.m_ovRecv.OffsetHigh += lr.HighPart;

	std::wcout << dwTransferred << L" Bytes recv." << std::endl;

	PP_PACKET packetRecv = {};
	packetRecv.m_socketSession = Session.m_socketSession;
	memcpy(packetRecv.m_packet.m_msg, Session.m_wsabufRecv.buf, dwTransferred);
	std::cout << packetRecv.m_packet.m_msg << std::endl;

	iReturn = Sender.Send(Session, dwTransferred);
	if (iReturn != 0) {
		return -1;
	}
	iReturn = Receiver.Receive(Session, BUFFER_SIZE);
	if (iReturn != 0) {
		return -1;
	}

	return 0;
}

int PPIOCPWorker::DispatchSend(PPSession Session, DWORD dwTransferred)
{
	LARGE_INTEGER lr;
	lr.QuadPart = dwTransferred;
	Session.m_ovSend.Offset += lr.LowPart;
	Session.m_ovSend.OffsetHigh += lr.HighPart;

	std::wcout << dwTransferred << L" Bytes send." << std::endl;

	return 0;
}
