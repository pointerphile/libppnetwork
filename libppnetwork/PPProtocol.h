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
	//��Ŷ �з��� ���� ������ ����
	//PPPacket::m_Header::m_type�� �����մϴ�.
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
	//���Ź��� ��Ŷ���� �۽��� ��Ŷ���� ǥ���ϴ� ������ ����
	enum PPPacketMode : unsigned short {
		RECV,
		SEND,
	};
#pragma pack(push, 1)
	//��Ŷ ���
	struct PPPacketHeader {
		unsigned short m_len;				//��Ŷ�� ���� 2����Ʈ(��� ����(PACKET_HEADER_SIZE)���� �����ؾ� ��)
		PPPacketType m_type;				//��Ŷ�� ���� 2����Ʈ
	};
	//��Ŷ ��� + ��ü
	struct PPPacket {
		PPPacketHeader m_Header;			//��Ŷ ��� 4����Ʈ
		char m_Payload[PACKET_PAYLOAD_SIZE];		//��Ŷ ��ü �ִ� 2048 ����Ʈ
	};
	//���ڿ� ������ ���� ����ü
	struct PPPacketMessage {
		char m_charMessage[PACKET_PAYLOAD_SIZE];	//�ִ� 2048 ����Ʈ�� ���ڿ� ���� ����
	};
	struct PPPacketNoticeSessionExit {
		SOCKET m_socketSession;
	};
	struct PPPacketNoticeSession {
		SOCKET m_socketSession;
	};
#pragma pack(pop)
	//������� ó���� ��Ŷ
	struct PPPacketForProcess {
		SOCKET m_socketSession;				//��Ŷ�� ������ ������ ����
		PPPacketMode m_Mode;				//���Ź��� ��Ŷ���� �۽��� ��Ŷ���� ǥ���ϴ� ������ ����
		PPPacket m_Packet;					//���� ���� ��Ŷ
	};
}