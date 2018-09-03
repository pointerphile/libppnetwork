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
	while (!m_isShutdown) {
		bool isReturn = 0;
		DWORD dwTransferred = 0;
		ULONG_PTR lpCompletionKey = 0;
		LPOVERLAPPED lpOverlapped = nullptr;
		
		isReturn = GetQueuedCompletionStatus(
			PPIOCP::GetInstance().m_hIOCP,
			&dwTransferred,
			&lpCompletionKey,
			&lpOverlapped,
			INFINITE
		);
		PPSession* pSession = (PPSession*)lpCompletionKey;

		if (isReturn == true) {
			if (dwTransferred == 0) {
				PPSessionManager::GetInstance().erase(pSession->m_socketSession);
			}
			if (dwTransferred != 0 && lpOverlapped != 0) {
				Receiver.Receive(pSession, dwTransferred);
			}
		}
		else {
			if (GetLastError() != ERROR_OPERATION_ABORTED) {
				if (dwTransferred == 0 && pSession != nullptr) {
					PPSessionManager::GetInstance().erase(pSession->m_socketSession);
				}
			}
		}
	}
	std::cout << "PPIOCPWorker::Release()" << std::endl;
	return 0;
}

int PPIOCPWorker::Release() { return 0; }
