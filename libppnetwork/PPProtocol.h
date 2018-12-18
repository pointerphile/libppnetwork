#pragma once
constexpr auto PACKET_BUFFER_SIZE = 2048;
constexpr auto PACKET_HEADER_SIZE = 4;

namespace PP {
#pragma pack(push, 1)
	enum PPPacketType : unsigned short {
		TYPE_NONE,
		TYPE_STRING,
		TYPE_MOVE
	};
	//PPSender에서 전송방법을 결정하기 위한 열거형 변수
	enum PPSendMode : unsigned short {
		NONE,
		SEND,
		BROADCAST
	};
	//패킷 헤더
	struct PPPacketHeader {
		unsigned short m_len;				//패킷의 길이 2바이트
		unsigned short m_type;				//패킷의 종류 2바이트
	};
	//패킷 헤더 + 본체
	struct PPPacket {
		PPPacketHeader m_Header;			//패킷 헤더 4바이트
		char m_Payload[PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE];		//패킷 본체 최대 2048 바이트
	};
	//문자열 전송을 위한 구조체
	struct PPPacketMessage {
		char m_charMessage[PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE];	//최대 2048 바이트의 문자열 전송 가능
	};
	//사용자명, 비밀번호 전송을 위한 구조체
	struct PPPacketAccount {
		char m_charUsername[16];			//사용자명 16바이트
		char m_charPassword[16];			//비밀번호 16바이트
	};

	struct PPPacketMove {
		float m_fNormalx;
		float m_fNormaly;
		float m_fNormalz;
		float m_fSpeed;
	};
#pragma pack(pop)
	//PPRecv로 받은 패킷
	struct PPRecvPacket {
		SOCKET m_socketSession;				//패킷을 받은 세션의 소켓
		PPPacket m_Packet;					//실제 보낼 패킷
	};
	//PPSender로 보내기 위한 패킷
	struct PPSendPacket {
		SOCKET m_socketSession;				//패킷을 보내는 세션의 소켓
		PPSendMode m_SendMode;				//PPSender에서 전송방법을 결정하기 위한 열거형 변수
		PPPacket m_Packet;					//실제 보낼 패킷
	};
}

