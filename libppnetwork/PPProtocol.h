#pragma once
constexpr auto PACKET_BUFFER_SIZE = 2048;
constexpr auto PACKET_HEADER_SIZE = 4;
constexpr auto PACKET_PAYLOAD_SIZE = PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE;
//List of Data Types
//1 Byte	:	bool, char
//2 Bytes	:	short, wchar_t(Windows)
//4 Bytes	:	int, float, long
//8 Bytes	:	long long
namespace PP {
	//패킷 분류를 위한 열거형 변수
	//PPPacket::m_Header::m_type에 지정합니다.
	enum PPPacketType : unsigned short {
		TYPE_NONE,							//Default
		TYPE_STRING,
		TYPE_STRING_HOST_TO_GUEST,
		TYPE_STRING_GUEST_TO_HOST,
		TYPE_NOTICE_SESSION_EXIT,
		TYPE_REQ_BROADCAST_EXCEPT_ME,
		TYPE_REQ_SOCKET,
		TYPE_ACK_SOCKET
	};
	//수신받은 패킷인지 송신할 패킷인지 표시하는 열거형 변수
	enum PPPacketMode : unsigned short {
		RECV,
		SEND,
	};
#pragma pack(push, 1)
	//패킷 헤더
	struct PPPacketHeader {
		unsigned short m_len;				//패킷의 길이 2바이트(헤더 길이(PACKET_HEADER_SIZE)까지 포함해야 함)
		PPPacketType m_type;				//패킷의 종류 2바이트
	};
	//패킷 헤더 + 본체
	struct PPPacket {
		PPPacketHeader m_Header;			//패킷 헤더 4바이트
		char m_Payload[PACKET_PAYLOAD_SIZE];		//패킷 본체 최대 2048 바이트
	};
	//문자열 전송을 위한 구조체
	struct PPPacketMessage {
		char m_charMessage[PACKET_PAYLOAD_SIZE];	//최대 2048 바이트의 문자열 전송 가능
	};
	struct PPPacketNoticeSessionExit {
		SOCKET m_socketSession;
	};
	struct PPPacketNoticeSession {
		SOCKET m_socketSession;
	};
#pragma pack(pop)
	//응용계층 처리용 패킷
	struct PPPacketForProcess {
		SOCKET m_socketSession;				//패킷을 보내는 세션의 소켓
		PPPacketMode m_Mode;				//수신받은 패킷인지 송신할 패킷인지 표시하는 열거형 변수
		PPPacket m_Packet;					//실제 보낼 패킷
	};
}