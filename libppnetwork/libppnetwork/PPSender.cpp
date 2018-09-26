#include "PPSender.h"
#include "PPSendPacketPool.h"
#include "PPSessionManager.h"

PPSender::PPSender() {}
PPSender::~PPSender() {}

int PPSender::Init() { return 0; }
int PPSender::Run() { return 0; }
int PPSender::Release() { return 0; }

int PPSender::Send(PPSession Session, DWORD dwBytesToWrite) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	Session.m_wsabufSend.buf = Session.m_bufWrite;
	Session.m_wsabufSend.len = dwBytesToWrite;

	//isReturn = WriteFile(m_hFileWrite, m_bufData, dwBytesToWrite, &dwBytesWritten, (LPOVERLAPPED)&m_ovWrite);
	isReturn = WSASend(Session.m_socketSession, &Session.m_wsabufSend, 1, nullptr, 0, &Session.m_ovSend, nullptr);
	if (isReturn == false) {
		dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
			DisplayError(L"WriteFile()");

			return -1;
		}
	}

	return 0;
}

int PPSender::SendFromSendPacketPool() {
	if (PPSendPacketPool::GetInstance().size()) {
		std::lock_guard<std::mutex> lock(m_mutexThis);
		int iResult = 0;
		DWORD flags = 0;
		PP_PACKET packet = {};
		std::string strTemp;

		if (PPSendPacketPool::GetInstance().size()) {
			packet = PPSendPacketPool::GetInstance().front();
		}
		strTemp = std::to_string(packet.m_socketSession);
		strTemp.append(" : ");
		strTemp.append(packet.m_packet.m_msg);

		for (std::map<SOCKET, PPSession>::iterator iter = PPSessionManager::GetInstance().begin();
			iter != PPSessionManager::GetInstance().end();
			++iter) {
			//iResult = send(iter->first, packet.m_packet.m_msg, (int)strlen(packet.m_packet.m_msg) + 1, 0);
			iResult = send(iter->first, strTemp.c_str(), (int)strTemp.length(), 0);
			if (iResult == SOCKET_ERROR) {
				DisplayError(L"send()");
				return -1;
			}
		}

		PPSendPacketPool::GetInstance().pop_front();
		//if (packet.m_pSession->m_socketClient != INVALID_SOCKET) {
		//	std::cout << "socket " << packet.m_pSession->m_socketClient
		//		<< " Sended : " << packet.m_packet.m_msg << std::endl;
		//}
	}
	else {
		std::cout << "PPSendPacketPool is empty." << std::endl;
	}
	return 0;
}

int PPSender::Broadcast(PPSession Session, DWORD dwBytesToWrite) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	Session.m_wsabufSend.buf = Session.m_bufWrite;
	Session.m_wsabufSend.len = dwBytesToWrite;

	for (std::map<SOCKET, PPSession>::iterator iter = PPSessionManager::GetInstance().begin();
		iter != PPSessionManager::GetInstance().end();
		++iter) {
		iter->second.m_wsabufSend.buf = Session.m_bufWrite;
		iter->second.m_wsabufSend.len = dwBytesToWrite;

		isReturn = WSASend(iter->second.m_socketSession, &iter->second.m_wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (isReturn == false) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WriteFile()");

				return -1;
			}
		}
	}
	return 0;
}

int PPSender::BroadcastFromSendPacketPool() {
	if (PPSendPacketPool::GetInstance().size()) {
		std::lock_guard<std::mutex> lock(m_mutexThis);
		int iResult = 0;
		DWORD flags = 0;
		PP_PACKET packet = { 0 };
		std::string strTemp;
		
		if (PPSendPacketPool::GetInstance().size()) {
			packet = PPSendPacketPool::GetInstance().front();
		}
		strTemp = std::to_string(packet.m_socketSession);
		strTemp.append(" : ");
		strTemp.append(packet.m_packet.m_msg);

		for (std::map<SOCKET, PPSession>::iterator iter = PPSessionManager::GetInstance().begin();
			iter != PPSessionManager::GetInstance().end();
			++iter) {
			//iResult = send(iter->first, packet.m_packet.m_msg, (int)strlen(packet.m_packet.m_msg) + 1, 0);
			iResult = send(iter->first, strTemp.c_str(), (int)strTemp.length(), 0);
			if (iResult == SOCKET_ERROR) {
				DisplayError(L"send()");
				return -1;
			}
		}
		
		PPSendPacketPool::GetInstance().pop_front();
	}
	else {
		std::cout << "PPSendPacketPool is empty." << std::endl;
	}

	return 0;
}