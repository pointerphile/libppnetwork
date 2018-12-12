#pragma once
constexpr auto BUFFER_SIZE = 2048;

#pragma pack(push, 1)
namespace PP {
	enum enumSendMode {NONE, SEND, BROADCAST};
	//패킷 헤더
	struct PPPacketHeader {
		unsigned short m_len;			//패킷의 길이 2바이트
		unsigned short m_type;			//패킷의 종류 2바이트
	};
	//패킷
	struct PPPacket {
		PPPacketHeader m_Header;			//패킷 헤더 4바이트
		char m_Payload[BUFFER_SIZE];	//패킷 본체 최대 2048 바이트
	};

	struct PPUnprocessedPacket {
		PPPacket m_Packet;
		enumSendMode m_SendMode;
	};

	struct PPPacketAccount {
		char m_charUsername[16];		//사용자명 16바이트
		char m_charPassword[16];		//비밀번호 16바이트
	};

	struct PPPacketMessage {
		char m_charMessage[BUFFER_SIZE];
	};
}
#pragma pack(pop)
