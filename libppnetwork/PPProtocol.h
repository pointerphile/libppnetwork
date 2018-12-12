#pragma once
constexpr auto BUFFER_SIZE = 2048;

#pragma pack(push, 1)
namespace PP {
	enum enumSendMode {NONE, SEND, BROADCAST};
	//��Ŷ ���
	struct PPPacketHeader {
		unsigned short m_len;			//��Ŷ�� ���� 2����Ʈ
		unsigned short m_type;			//��Ŷ�� ���� 2����Ʈ
	};
	//��Ŷ
	struct PPPacket {
		PPPacketHeader m_Header;			//��Ŷ ��� 4����Ʈ
		char m_Payload[BUFFER_SIZE];	//��Ŷ ��ü �ִ� 2048 ����Ʈ
	};

	struct PPUnprocessedPacket {
		PPPacket m_Packet;
		enumSendMode m_SendMode;
	};

	struct PPPacketAccount {
		char m_charUsername[16];		//����ڸ� 16����Ʈ
		char m_charPassword[16];		//��й�ȣ 16����Ʈ
	};

	struct PPPacketMessage {
		char m_charMessage[BUFFER_SIZE];
	};
}
#pragma pack(pop)
