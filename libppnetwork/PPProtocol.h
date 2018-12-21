#pragma once
constexpr auto PACKET_BUFFER_SIZE = 2048;
constexpr auto PACKET_HEADER_SIZE = 4;

namespace PP {
#pragma pack(push, 1)
	//��Ŷ �з��� ���� ������ ����
	enum PPPacketType : unsigned short {
		TYPE_NONE,
		TYPE_STRING,
		TYPE_REQ_OBJECT_LIST,
		TYPE_ACK_OBJECT_LIST,
		TYPE_MOVE_HOST_TO_GUEST,
		TYPE_MOVE_GUEST_TO_HOST,
	};
	//PPSender���� ���۹���� �����ϱ� ���� ������ ����
	enum PPSendMode : unsigned short {
		RECV,
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
		char m_Payload[PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE];		//��Ŷ ��ü �ִ� 2048 ����Ʈ
	};
	//���ڿ� ������ ���� ����ü
	struct PPPacketMessage {
		char m_charMessage[PACKET_BUFFER_SIZE - PACKET_HEADER_SIZE];	//�ִ� 2048 ����Ʈ�� ���ڿ� ���� ����
	};
	//����ڸ�, ��й�ȣ ������ ���� ����ü
	struct PPPacketAccount {
		char m_charUsername[16];			//����ڸ� 16����Ʈ
		char m_charPassword[16];			//��й�ȣ 16����Ʈ
	};
	//ȣ��Ʈ���� �Խ�Ʈ�� ������Ʈ �������� ���۵��� �˸��� ��Ŷ
	//����: �Խ�Ʈ->ȣ��Ʈ
	struct PPPacketStartMoveObjectGuestToHost {
		short m_iObjectID;					//������Ʈ ID
		float m_fNormalx;					//������Ʈ �̵����� x
		float m_fNormaly;					//������Ʈ �̵����� y
		float m_fNormalz;					//������Ʈ �̵����� z
		float m_fSpeed;						//������Ʈ �̵��ӵ�
	};
	//�Խ�Ʈ���� ȣ��Ʈ�� ������Ʈ �������� ���۵��� �˸��� ��Ŷ
	//����: ȣ��Ʈ->�Խ�Ʈ
	struct PPPacketStartMoveObjectHostToGuest {
		short m_iObjectID;					//������Ʈ ID
		float m_fNormalx; 					//������Ʈ �̵�����
		float m_fNormaly; 					//������Ʈ �̵�����
		float m_fNormalz; 					//������Ʈ �̵�����
		float m_fSpeed;	  					//������Ʈ �̵��ӵ�
	};
	//ȣ��Ʈ���� �Խ�Ʈ�� ������Ʈ �������� ������ �˸��� ��Ŷ
	//����: �Խ�Ʈ->ȣ��Ʈ
	struct PPPacketStopMoveObjectGuestToHost {
		int m_iObjectID;
	};
	struct PPPacketStopMoveObjectHostToGuest {
		int m_iObjectID;
	};
#pragma pack(pop)
	//ó���� ��Ŷ
	struct PPPacketForProcess {
		SOCKET m_socketSession;				//��Ŷ�� ������ ������ ����
		PPSendMode m_SendMode;				//PPSender���� ���۹���� �����ϱ� ���� ������ ����
		PPPacket m_Packet;					//���� ���� ��Ŷ
	};
}

