#pragma once
#define BUFFER_SIZE 2048
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
#pragma pack(pop)

#define PACKET_HEADER_SIZE 4

#define PACKET_CHAT_WELCOME_REQ 0000
#define PACKET_CHAT_WELCOME_ACK 0001
#define PACKET_CHAT_USERNAME_REQ 1000
#define PACKET_CHAT_USERNAME_ACK 1001
#define PACKET_CHAT_PASSWORD_REQ 2000
#define PACKET_CHAT_PASSWORD_ACK 2001

#define PACKET_CHAT_MSG 3000