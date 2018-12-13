#pragma once
constexpr auto BUFFER_SIZE = 2048;

#pragma pack(push, 1)
namespace PP {
	enum PPPacketType : unsigned short {
		STRING
	};

	//PPSender에서 전송방법을 결정하기 위한 열거형 변수
	enum PPSendMode {
		NONE,
		SEND,
		BROADCAST
	};
	//패킷 헤더
	struct PPPacketHeader {
		unsigned short m_len;				//패킷의 길이 2바이트
		unsigned short m_type;				//패킷의 종류 2바이트
	};
	//패킷
	struct PPPacket {
		PPPacketHeader m_Header;			//패킷 헤더 4바이트
		char m_Payload[BUFFER_SIZE];		//패킷 본체 최대 2048 바이트
	};
	//PPSender로 보내기 위한 패킷
	struct PPSendPacket {
		PPPacket m_Packet;					//실제 보낼 패킷
		PPSendMode m_SendMode;				//PPSender에서 전송방법을 결정하기 위한 열거형 변수
	};
	//문자열 전송을 위한 구조체
	struct PPPacketMessage {
		char m_charMessage[BUFFER_SIZE];	//최대 2048 바이트의 문자열 전송 가능
	};
	//사용자명, 비밀번호 전송을 위한 구조체
	struct PPPacketAccount {
		char m_charUsername[16];			//사용자명 16바이트
		char m_charPassword[16];			//비밀번호 16바이트
	};
}
#pragma pack(pop)
