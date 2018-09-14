#pragma once
constexpr auto BUFFER_SIZE = 2048;
#pragma pack(push, 1)
class PACKET_HEADER {
public:
	unsigned short m_len;
	unsigned short m_type;
};

class UPACKET {
public:
	PACKET_HEADER m_ph;
	char m_msg[BUFFER_SIZE];
};

class PACKET_ACCOUNT {
public:
	char m_strUsername[16];
	char m_strPassword[16];
};
#pragma pack(pop)

constexpr auto PACKET_HEADER_SIZE = 4;

constexpr auto PACKET_WELCOME_REQ = 1000;
constexpr auto PACKET_WELCOME_ACK = 1001;
constexpr auto PACKET_ACCOUNT_REQ = 2000;
constexpr auto PACKET_ACCOUNT_ACK = 2001;

constexpr auto PACKET_CHAT_MSG = 4000;