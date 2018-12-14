#include "PPIOCP.h"
#include "PPSessionManager.h"
#include "PPRecvPacketPool.h"
#include "PPSendPacketPool.h"

PP::PPIOCP::PPIOCP() {}
PP::PPIOCP::~PPIOCP() {}

int PP::PPIOCP::Init() {
	std::wcout << L"PPIOCP::Init()..." << std::endl;
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
	bool isReturn = true;
	DWORD dwTransferred = 0;
	unsigned __int64 lpCompletionKey;//키 값은 각 세션의 소켓을 사용
	PPOVERLAPPED* overlapped;
	while (true) {
		lpCompletionKey = 0;
		overlapped = 0;

		isReturn = GetQueuedCompletionStatus(m_hIOCP, &dwTransferred, &lpCompletionKey, (LPOVERLAPPED*)&overlapped, INFINITE);
		//넘겨받은 키 값으로 세션 탐색
		auto iter = PPSessionManager::GetInstance().find((SOCKET)lpCompletionKey);
		PPSessionManager::GetInstance().end();
		if (iter == PPSessionManager::GetInstance().end()) {
			DisplayError(L"GetQueuedCompletionStatus");
			continue;
		}

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
				//dwTransferred == 0
				//세션 종료시 현재 접속자들에게 해당 세션이 접속을 종료하였음을 Broadcast 후
				//세션 리스트에서 해당 세션 제거 실시
				DisplayError(L"GetQueuedCompletionStatus()");
				//PP_PACKET packetSend = {};
				//std::string strBuf;
				//packetSend.m_packet.m_ph.m_type = PACKET_CHAT_MSG;
				//strBuf.append(iter->second.m_strUsername);
				//strBuf.append(" left from this chat.");
				//memcpy(packetSend.m_packet.m_msg, strBuf.c_str(), strBuf.size());
				//packetSend.m_packet.m_ph.m_len = PACKET_HEADER_SIZE + (unsigned short)strBuf.size();
				//memcpy(iter->second.m_bufWrite, (void*)&packetSend.m_packet, packetSend.m_packet.m_ph.m_len);
				//Sender.Broadcast(iter->second, packetSend.m_packet.m_ph.m_len);
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
					//클라이언트 비정상종료로 간주
					//현재 접속자들에게 해당 세션이 접속을 종료하였음을 Broadcast 후
					//세션 리스트에서 해당 세션 제거 실시
					DisplayError(L"GetQueuedCompletionStatus()");
					//PP_PACKET packetSend = {};
					//std::string strBuf;
					//packetSend.m_packet.m_ph.m_type = PACKET_CHAT_MSG;
					//strBuf.append(iter->second.m_strUsername);
					//strBuf.append(" left from this chat.");
					//memcpy(packetSend.m_packet.m_msg, strBuf.c_str(), strBuf.size());
					//packetSend.m_packet.m_ph.m_len = PACKET_HEADER_SIZE + (unsigned short)strBuf.size();
					//memcpy(iter->second.m_bufWrite, (void*)&packetSend.m_packet, packetSend.m_packet.m_ph.m_len);
					//Sender.Broadcast(iter->second, packetSend.m_packet.m_ph.m_len);
					PPSessionManager::GetInstance().erase(iter->first);
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
	Session.m_ovRecv.Offset += lr.LowPart;
	Session.m_ovRecv.OffsetHigh += lr.HighPart;

	std::wcout << dwTransferred << L" Bytes recv." << std::endl;

	PPRecvPacket packetRecv = {};
	PPSendPacket packetSend = {};
	packetRecv.m_socketSession = Session.m_socketSession;
	memcpy((void*)&packetRecv, Session.m_wsabufRecv.buf, dwTransferred);
	PPRecvPacketPool::GetInstance().m_listRecvPacket.push_back(packetRecv);

	//
	//수신받은 패킷 처리 부분
	//

	//
	//
	//while (PPSendPacketPool::GetInstance().empty() == false) {
	//	packetSend = PPSendPacketPool::GetInstance().front();
	//	PPSendPacketPool::GetInstance().pop_front();
	//	memcpy(Session.m_bufWrite, (void*)&packetSend.m_packet, packetSend.m_packet.m_ph.m_len);


		//Send할 패킷과 Broadcast할 패킷 분류 후 전송
		//if (packetSend.m_packet.m_ph.m_type == PACKET_CHAT_MSG) {
		//	iReturn = Sender.Broadcast(Session, packetSend.m_packet.m_ph.m_len);
		//}
		//else {
		//	iReturn = Sender.Send(Session, packetSend.m_packet.m_ph.m_len);
		//}
		//if (iReturn != 0) {
			return -1;
		//}

	//}

	//WSAReceive 실시
	iReturn = Receiver.Recv(Session, PACKET_BUFFER_SIZE);
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

	std::wcout << dwTransferred << L" Bytes send." << std::endl;
	return 0;
}

int PP::PPIOCP::SetNumberOfWorkers(unsigned short iNumberOfThreads) {
	m_iNumberOfThreads = iNumberOfThreads;
	return 0;
}
