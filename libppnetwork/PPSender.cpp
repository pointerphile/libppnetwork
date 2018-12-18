#include "PPSender.h"
#include "PPSessionManager.h"

PP::PPSender::PPSender() {}
PP::PPSender::~PPSender() {}

int PP::PPSender::Init() { return 0; }
int PP::PPSender::Run() { return 0; }
int PP::PPSender::Release() { return 0; }

int PP::PPSender::Send(PPSession Session, DWORD dwBytesToWrite) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	Session.m_wsabufSend.buf = Session.m_bufWrite;
	Session.m_wsabufSend.len = dwBytesToWrite;

	isReturn = WSASend(Session.m_socketSession, &Session.m_wsabufSend, 1, nullptr, 0, &Session.m_ovSend, nullptr);
	if (isReturn == false) {
		dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
			DisplayError(L"WSASend()");
			return -1;
		}
	}

	return 0;
}

int PP::PPSender::SendRawString(PPSession Session, std::wstring wstrMessage)
{
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	WSABUF wsabufSend = {};
	//WSABUF로 패킷 복사
	wsabufSend.buf = (char*)wstrMessage.c_str();
	wsabufSend.len = (ULONG)wstrMessage.size() * 2;

	isReturn = WSASend(Session.m_socketSession, &wsabufSend, 1, nullptr, 0, &Session.m_ovSend, nullptr);
	if (isReturn == false) {
		dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
			DisplayError(L"WSASend()");
			return -1;
		}
	}
	return 0;
}

int PP::PPSender::Broadcast(PPSession Session, DWORD dwBytesToWrite) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	Session.m_wsabufSend.buf = Session.m_bufWrite;
	Session.m_wsabufSend.len = dwBytesToWrite;

	for (auto iter = PPSessionManager::GetInstance().begin();
		iter != PPSessionManager::GetInstance().end();
		++iter) {
		iter->second.m_wsabufSend.buf = Session.m_bufWrite;
		iter->second.m_wsabufSend.len = dwBytesToWrite;

		isReturn = WSASend(iter->second.m_socketSession, &iter->second.m_wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (isReturn == false) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WSASend()");
				return -1;
			}
		}
	}
	return 0;
}

int PP::PPSender::Broadcast(PPSendPacket packetSend) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	WSABUF wsabufSend = {};

	//WSABUF로 패킷 지정
	wsabufSend.buf = (char*)&packetSend.m_Packet;
	wsabufSend.len = packetSend.m_Packet.m_Header.m_len;

	//전체 세션 순회
	if (PPSessionManager::GetInstance().m_mapSession.empty()) {
		return -1;
	}
	for (auto iter = PPSessionManager::GetInstance().begin();
		iter != PPSessionManager::GetInstance().end();
		++iter) {

		isReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (isReturn == false) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WSASend()");
				return -1;
			}
		}
	}
	return 0;
}

int PP::PPSender::BroadcastWString(std::wstring wstrMessage) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	WSABUF wsabufSend = {};
	PPSendPacket packetSend = {};

	//패킷 작성
	packetSend.m_SendMode = PPSendMode::BROADCAST;
	packetSend.m_socketSession = 0;
	memcpy(packetSend.m_Packet.m_Payload, wstrMessage.c_str(), wstrMessage.size() * 2);
	packetSend.m_Packet.m_Header.m_type = PPPacketType::TYPE_STRING;
	packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + ((unsigned short)wstrMessage.size() * 2);

	//WSABUF로 패킷 지정
	wsabufSend.buf = (char*)&packetSend.m_Packet;
	wsabufSend.len = packetSend.m_Packet.m_Header.m_len;

	//전체 세션 순회
	if (PPSessionManager::GetInstance().m_mapSession.empty()) {
		return -1;
	}
	for (auto iter = PPSessionManager::GetInstance().begin();
		iter != PPSessionManager::GetInstance().end();
		++iter) {

		isReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (isReturn == false) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WSASend()");
				return -1;
			}
		}
	}
	return 0;
}

int PP::PPSender::BroadcastRawWString(std::wstring wstrMessage) {
	bool isReturn = false;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	std::wstring wstrBuf;
	WSABUF wsabufSend = {};

	//WSABUF로 패킷 복사
	wsabufSend.buf = (char*)wstrMessage.c_str();
	wsabufSend.len = (ULONG)wstrMessage.size() * 2;

	//전체 세션 순회
	for (auto iter = PPSessionManager::GetInstance().begin();
		iter != PPSessionManager::GetInstance().end();
		++iter) {

		isReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (isReturn == false) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING && dwError != ERROR_SUCCESS) {
				DisplayError(L"WSASend()");
				return -1;
			}
		}
	}
	return 0;
}

LIBPPNETWORK_API PP::PPSender * PP::GetSender()
{
	return new PP::PPSender();
}
