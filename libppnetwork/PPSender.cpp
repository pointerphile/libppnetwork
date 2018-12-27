#include "PPSender.h"
#include "PPSessionManager.h"

PP::PPSender::PPSender() {}
PP::PPSender::~PPSender() {}

int PP::PPSender::Init() { return 0; }
int PP::PPSender::Run() { return 0; }
int PP::PPSender::Release() { return 0; }

LIBPPNETWORK_API int PP::PPSender::Send(PPPacketForProcess packetSend) {
	int iReturn = 0;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	WSABUF wsabufSend = {};

	//WSABUF로 패킷 지정
	wsabufSend.buf = (char*)&packetSend.m_Packet;
	wsabufSend.len = packetSend.m_Packet.m_Header.m_len;

	auto iter = PPSessionManager::GetInstance().find(packetSend.m_socketSession);

	iReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
	if (iReturn == SOCKET_ERROR) {
		dwError = WSAGetLastError();
		if (dwError != WSA_IO_PENDING) {
			DisplayError(L"WSASend()");
			//return -1;
		}
	}
	return iReturn;
}

LIBPPNETWORK_API int PP::PPSender::SendToServer(PPPacketForProcess packetSend) {
	return Broadcast(packetSend);
}

int PP::PPSender::SendWStringToServer(std::wstring wstrMessage) {
	return BroadcastWString(wstrMessage);
}

int PP::PPSender::Broadcast(PPPacketForProcess packetSend) {
	int iReturn = 0;
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

		iReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, &dwBytesWritten, 0, &iter->second.m_ovSend, nullptr);
		if (iReturn == SOCKET_ERROR) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING) {
				DisplayError(L"WSASend()");
				//PPItachHunter::GetInstance().push_back(iter->second.m_socketSession);
			}
		}
	}
	return iReturn;
}

//PP::PPSender::BroadcastExcept()
//파라미터 : struct PP::PPPacketForProcess
//세션 리스트를 순회하여 PPPacketForProcess에 저장된 세션을 제외한
//나머지 세션들에게 WSASend를 호출합니다.
LIBPPNETWORK_API int PP::PPSender::BroadcastExcept(PPPacketForProcess packetSend) {
	int iReturn = 0;
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

		if (iter->second.m_socketSession != packetSend.m_socketSession) {
			iReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, &dwBytesWritten, 0, &iter->second.m_ovSend, nullptr);
			if (iReturn == SOCKET_ERROR) {
				dwError = WSAGetLastError();
				if (dwError != WSA_IO_PENDING) {
					DisplayError(L"WSASend()");
					//return -1;
				}
			}
		}
	}

	return iReturn;
}

int PP::PPSender::BroadcastWString(std::wstring wstrMessage) {
	int iReturn = 0;
	DWORD dwBytesWritten = 0;
	DWORD dwError = 0;
	WSABUF wsabufSend = {};
	PPPacketForProcess packetSend = {};

	//패킷 작성
	packetSend.m_Mode = PPPacketMode::SEND;
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

		iReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (iReturn == SOCKET_ERROR) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING) {
				DisplayError(L"WSASend()");
			}
		}
	}
	return iReturn;
}

int PP::PPSender::BroadcastRawWString(std::wstring wstrMessage) {
	int iReturn = 0;
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

		iReturn = WSASend(iter->second.m_socketSession, &wsabufSend, 1, nullptr, 0, &iter->second.m_ovSend, nullptr);
		if (iReturn == SOCKET_ERROR) {
			dwError = WSAGetLastError();
			if (dwError != WSA_IO_PENDING) {
				DisplayError(L"WSASend()");
			}
		}
	}
	return iReturn;
}

LIBPPNETWORK_API PP::PPSender * PP::GetSender()
{
	return new PP::PPSender();
}
