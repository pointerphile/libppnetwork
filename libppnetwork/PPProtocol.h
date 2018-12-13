#pragma once
constexpr auto BUFFER_SIZE = 2048;

namespace PP {
#pragma pack(push, 1)
	enum PPPacketType : unsigned short {
		STRING
	};
	//PPSender���� ���۹���� �����ϱ� ���� ������ ����
	enum PPSendMode {
		NONE,
		SEND,
		BROADCAST
	};
	//��Ŷ ���
	struct PPPacketHeader {
		unsigned short m_len;				//��Ŷ�� ���� 2����Ʈ
		unsigned short m_type;				//��Ŷ�� ���� 2����Ʈ
	};
	//��Ŷ ��� + ��ü
	struct PPPacket {
		PPPacketHeader m_Header;			//��Ŷ ��� 4����Ʈ
		char m_Payload[BUFFER_SIZE];		//��Ŷ ��ü �ִ� 2048 ����Ʈ
	};
	//���ڿ� ������ ���� ����ü
	struct PPPacketMessage {
		char m_charMessage[BUFFER_SIZE];	//�ִ� 2048 ����Ʈ�� ���ڿ� ���� ����
	};
	//����ڸ�, ��й�ȣ ������ ���� ����ü
	struct PPPacketAccount {
		char m_charUsername[16];			//����ڸ� 16����Ʈ
		char m_charPassword[16];			//��й�ȣ 16����Ʈ
	};
#pragma pack(pop)
	//PPRecv�� ���� ��Ŷ
	struct PPRecvPacket {
		SOCKET m_socketSession;				//��Ŷ�� ���� ������ ����
		PPPacket m_Packet;					//���� ���� ��Ŷ
	};
	//PPSender�� ������ ���� ��Ŷ
	struct PPSendPacket {
		SOCKET m_socketSession;				//��Ŷ�� ������ ������ ����
		PPSendMode m_SendMode;				//PPSender���� ���۹���� �����ϱ� ���� ������ ����
		PPPacket m_Packet;					//���� ���� ��Ŷ
	};
}

