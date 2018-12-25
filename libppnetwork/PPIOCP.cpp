#include "PPIOCP.h"
#include "PPSessionManager.h"
#include "PPServerRecvPacketPool.h"
#include "PPClientRecvPacketPool.h"

PP::PPIOCP::PPIOCP() {}
PP::PPIOCP::~PPIOCP() {}

int PP::PPIOCP::Init() {
	std::wcout << wcharVersion << std::endl;
	OutputDebugStringW(wcharVersion);
	std::wcout << L"PPIOCP::Init()..." << std::endl;
	OutputDebugStringW(L"PPIOCP::Init()...\n");
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, m_iNumberOfThreads);

	for (int iNumberOfThread = m_iNumberOfThreads; iNumberOfThread > 0; iNumberOfThread--) {
		this->LaunchThread();
		Sleep(17);
	}
	Sleep(17);
	return 0;
}

int PP::PPIOCP::Run() {
	std::wcout << L"PPIOCP::Run()..." << std::endl;
	OutputDebugStringW(L"PPIOCP::Run()...\n");
	bool isReturn = true;
	DWORD dwTransferred = 0;
	unsigned __int64 lpCompletionKey;//키 값은 각 세션의 소켓을 사용
	PPOVERLAPPED* overlapped;
	while (true) {
		lpCompletionKey = 0;
		overlapped = 0;

		isReturn = GetQueuedCompletionStatus(m_hIOCP, &dwTransferred, &lpCompletionKey, (LPOVERLAPPED*)&overlapped, INFINITE);
		std::wcout << L"session number: " << PPSessionManager::GetInstance().size() << std::endl;
		//넘겨받은 키 값으로 세션 탐색
		auto iter = PPSessionManager::GetInstance().find((SOCKET)lpCompletionKey);
		if (iter == PPSessionManager::GetInstance().end()) {
			DisplayError(L"GetQueuedCompletionStatus");
			PPPacketForProcess packetSend;
			PPPacketNoticeSessionExit packetNotice;
			packetNotice.m_socketSession = lpCompletionKey;
			memcpy(packetSend.m_Packet.m_Payload, (void*)&packetNotice, sizeof(packetNotice));
			packetSend.m_Packet.m_Header.m_type = PPPacketType::TYPE_NOTICE_SESSION_EXIT;
			packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + sizeof(packetNotice);
			m_Sender.Broadcast(packetSend);

			auto iterDelete = PPSessionManager::GetInstance().find(lpCompletionKey);
			if (iterDelete != PPSessionManager::GetInstance().end()) {
				PPSessionManager::GetInstance().erase(lpCompletionKey);
			}
			continue;
		}
		if (isReturn == true) {
			DisplayError(L"GetQueuedCompletionStatus()");
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
				//dwTransferred == 0
				//세션 종료시 현재 접속자들에게 해당 세션이 접속을 종료하였음을 Broadcast 후
				//세션 리스트에서 해당 세션 제거 실시
				DisplayError(L"GetQueuedCompletionStatus()");
				PPPacketForProcess packetSend;
				PPPacketNoticeSessionExit packetNotice;
				packetNotice.m_socketSession = lpCompletionKey;
				memcpy(packetSend.m_Packet.m_Payload, (void*)&packetNotice, sizeof(packetNotice));
				packetSend.m_Packet.m_Header.m_type = PPPacketType::TYPE_NOTICE_SESSION_EXIT;
				packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + sizeof(packetNotice);
				m_Sender.Broadcast(packetSend);

				auto iterDelete = PPSessionManager::GetInstance().find(lpCompletionKey);
				if (iterDelete != PPSessionManager::GetInstance().end()) {
					PPSessionManager::GetInstance().erase(lpCompletionKey);
				}
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
					//클라이언트 비정상종료로 간주
					//현재 접속자들에게 해당 세션이 접속을 종료하였음을 Broadcast 후
					//세션 리스트에서 해당 세션 제거 실시
					DisplayError(L"GetQueuedCompletionStatus()");
					PPPacketForProcess packetSend;
					PPPacketNoticeSessionExit packetNotice;
					packetNotice.m_socketSession = lpCompletionKey;
					memcpy(packetSend.m_Packet.m_Payload, (void*)&packetNotice, sizeof(packetNotice));
					packetSend.m_Packet.m_Header.m_type = PPPacketType::TYPE_NOTICE_SESSION_EXIT;
					packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + sizeof(packetNotice);
					m_Sender.Broadcast(packetSend);

					auto iterDelete = PPSessionManager::GetInstance().find(lpCompletionKey);
					if (iterDelete != PPSessionManager::GetInstance().end()) {
						PPSessionManager::GetInstance().erase(lpCompletionKey);
					}
				}
				else {
					DisplayError(L"GetQueuedCompletionStatus()");
					exit(-1);
				}
			}
		}
	}

	return 0;
}

int PP::PPIOCP::Release() {
	CloseHandle(m_hIOCP);
	return 0;
}

HANDLE PP::PPIOCP::BindSocket(HANDLE handle, ULONG_PTR CompletionKey) {
	return CreateIoCompletionPort(handle, m_hIOCP, CompletionKey, 0);
}

int PP::PPIOCP::DispatchRecv(PPSession& Session, DWORD dwTransferred) {
	int iReturn = 0;
	LARGE_INTEGER lr;
	lr.QuadPart = dwTransferred;
	PPPacketForProcess packetRecv = {};
	wchar_t wcharBuf[1024] = {};

	Session.m_ovRecv.Offset += lr.LowPart;
	Session.m_ovRecv.OffsetHigh += lr.HighPart;
#ifdef TEST
	std::wcout << dwTransferred << L" Bytes recv." << std::endl;
#endif // TEST
	//WSARecv로 가져온 패킷 복사
	packetRecv.m_Mode = PPPacketMode::RECV;
	packetRecv.m_socketSession = Session.m_socketSession;
	memcpy((void*)&packetRecv.m_Packet, Session.m_wsabufRecv.buf, dwTransferred);
	if (bIsServer) {
		PPServerRecvPacketPool::GetInstance().push_back(packetRecv);
	}
	else {
		PPClientRecvPacketPool::GetInstance().push_back(packetRecv);
	}

	if (m_FP != nullptr) {
		m_FP();
	}

	//WSAReceive 실시
	iReturn = m_Receiver.Recv(Session, PACKET_BUFFER_SIZE);
	if (iReturn != 0) {
		return -1;
	}

	return 0;
}

int PP::PPIOCP::DispatchSend(PPSession& Session, DWORD dwTransferred) {
	LARGE_INTEGER lr;
	lr.QuadPart = dwTransferred;
	Session.m_ovSend.Offset += lr.LowPart;
	Session.m_ovSend.OffsetHigh += lr.HighPart;
#ifdef TEST
	std::wcout << dwTransferred << L" Bytes send." << std::endl;
#endif // TEST
	return 0;
}

int PP::PPIOCP::SetServer(bool boolean) {
	bIsServer = boolean;
	return 0;
}

int PP::PPIOCP::SetNumberOfWorkers(unsigned short iNumberOfThreads) {
	m_iNumberOfThreads = iNumberOfThreads;
	return 0;
}

LIBPPNETWORK_API int PP::PPIOCP::SetFP(int(*FP)() = nullptr) {
	m_FP = FP;
	return 0;
}