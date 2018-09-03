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
#define PACKET_CHAT_MSG 1000
#define PACKET_CHAT_NAME_REQ 2000
#define PACKET_CHAT_NAME_ACK 3000