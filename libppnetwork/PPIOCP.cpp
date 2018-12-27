#include "PPIOCP.h"
#include "PPSessionManager.h"
#include "PPServerRecvPacketPool.h"
#include "PPClientRecvPacketPool.h"
#include "PPItachHunter.h"

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
	bool isReturn = false;
	DWORD dwTransferred = 0;
	unsigned __int64 lpCompletionKey;//Ű ���� �� ������ ������ ���
	PPOVERLAPPED* overlapped;
	while (true) {
		isReturn = false;
		dwTransferred = 0;
		lpCompletionKey = 0;
		overlapped = 0;

		isReturn = GetQueuedCompletionStatus(m_hIOCP, &dwTransferred, &lpCompletionKey, (LPOVERLAPPED*)&overlapped, INFINITE);
		DWORD dwError = WSAGetLastError();
		//�Ѱܹ��� Ű ������ ���� Ž��
		auto iter = PPSessionManager::GetInstance().find((SOCKET)lpCompletionKey);
		if (iter == PPSessionManager::GetInstance().end()) {
			PPSessionManager::GetInstance().erase(lpCompletionKey);
			continue;
		}
		if (isReturn == true) {
			if (dwTransferred == 0) {
				//dwTransferred == 0
				//���� ����� ���� �����ڵ鿡�� �ش� ������ ������ �����Ͽ����� Broadcast ��
				//���� ����Ʈ���� �ش� ���� ���� �ǽ�
				dwError = WSAGetLastError();
				if (dwError != WSA_IO_PENDING) {
					std::wcout << iter->second.m_socketSession << L": ���� = T; ���۷� = 0; �������� ó��" << std::endl;
					DisplayError(L"GetQueuedCompletionStatus");
				}
				PPSessionManager::GetInstance().erase(lpCompletionKey);
				continue;
			}
			else {
				if (lpCompletionKey != 0 && overlapped != nullptr) {
					if (overlapped->dwFlag == ASYNCFLAG_SEND) {
						DispatchSend(iter->second, dwTransferred);
						dwError = WSAGetLastError();
						if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
							std::wcout << iter->second.m_socketSession << L": ���� = T; ���۷�&Ű&������ != 0; ASYNCFLAG_SEND" << std::endl;
							DisplayError(L"GetQueuedCompletionStatus");
						}
					}
					else {
						DispatchRecv(iter->second, dwTransferred);
						dwError = WSAGetLastError();
						if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
							std::wcout << iter->second.m_socketSession << L": ���� = T; ���۷�&Ű&������ != 0; ASYNCFLAG_RECV" << std::endl;
							DisplayError(L"GetQueuedCompletionStatus");
							PPSessionManager::GetInstance().erase(lpCompletionKey);
						}
					}
				}
			}
			
		}
		else {
			if (overlapped != nullptr) {
				//Ŭ���̾�Ʈ ����������� ����
				//���� �����ڵ鿡�� �ش� ������ ������ �����Ͽ����� Broadcast ��
				//���� ����Ʈ���� �ش� ���� ���� �ǽ�
				dwError = WSAGetLastError();
				if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
					std::wcout << iter->second.m_socketSession << L": ���� = F; ������ != 0 ���������� ó��" << std::endl;
					DisplayError(L"GetQueuedCompletionStatus");
				}
				PPSessionManager::GetInstance().erase(lpCompletionKey);
			}
			else {
				dwError = WSAGetLastError();
				if (dwError != WSA_IO_PENDING) {
					DisplayError(L"GetQueuedCompletionStatus");
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

int PP::PPIOCP::DispatchRecv(PPSession Session, DWORD dwTransferred) {
	int iReturn = 0;
	PPPacketForProcess packetRecv = {};
	wchar_t wcharBuf[1024] = {};

	std::wcout << Session .m_socketSession<< ": "<< dwTransferred << L" Bytes recv." << std::endl;
	//WSARecv�� ������ ��Ŷ ����
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

	//WSAReceive �ǽ�
	iReturn = m_Receiver.Recv(Session, PACKET_BUFFER_SIZE);

	return 0;
}

int PP::PPIOCP::DispatchSend(PPSession Session, DWORD dwTransferred) {
	std::wcout << Session.m_socketSession << ": " << dwTransferred << L" Bytes send." << std::endl;
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