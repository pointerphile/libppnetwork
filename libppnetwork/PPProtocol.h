#pragma once
constexpr auto PACKET_BUFFER_SIZE = 2048;
constexpr auto PACKET_HEADER_SIZE = 4;

namespace PP {
#pragma pack(push, 1)
	//패킷 분류를 위한 열거형 변수
	//PPPacket::m_Header::m_type에 지정합니다.
	enum PPPacketType : unsigned short {
		TYPE_NONE,
		TYPE_STRING,
		TYPE_STRING_HOST_TO_GUEST,
		TYPE_STRING_GUEST_TO_HOST,
		TYPE_REQ_OBJECT_LIST,
		TYPE_ACK_OBJECT_LIST,
		TYPE_MOVE_HOST_TO_GUEST,
		TYPE_MOVE_GUEST_TO_HOST,
		TYPE_NOTICE_SESSION_EXIT
	};
	//PPSender에서 전송방법을 결정하기 위한 열거형 변수
	enum PPSendMode : unsigned short {
		RECV,
		SEND,
		BROADCAST
	};
	//패킷 헤더
	struct PPPacketHeader {
		unsigned short m_len;				//패킷의 길이 2바이트(헤더 길이(PACKET_HEADER_SIZE)까지 포함해야 함)
		PPPacketType m_type;				//패킷의 종류 2바이트
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
	//호스트에게 게스트의 오브젝트 움직임이 시작됨을 알리는 패킷
	//방향: 게스트->호스트
	struct PPPacketStartMoveObjectGuestToHost {
		short m_iObjectID;					//오브젝트 ID
		float m_fNormalx;					//오브젝트 이동벡터 x
		float m_fNormaly;					//오브젝트 이동벡터 y
		float m_fNormalz;					//오브젝트 이동벡터 z
		float m_fSpeed;						//오브젝트 이동속도
	};
	//게스트에게 호스트의 오브젝트 움직임이 시작됨을 알리는 패킷
	//방향: 호스트->게스트
	struct PPPacketStartMoveObjectHostToGuest {
		short m_iObjectID;					//오브젝트 ID
		float m_fNormalx; 					//오브젝트 이동벡터
		float m_fNormaly; 					//오브젝트 이동벡터
		float m_fNormalz; 					//오브젝트 이동벡터
		float m_fSpeed;	  					//오브젝트 이동속도
	};
	//호스트에게 게스트의 오브젝트 움직임이 멈춤을 알리는 패킷
	//방향: 게스트->호스트
	struct PPPacketStopMoveObjectGuestToHost {
		int m_iObjectID;
	};
	struct PPPacketStopMoveObjectHostToGuest {
		int m_iObjectID;
	};
	struct PPPacketNoticeSessionExit {
		SOCKET m_socketSession;
	};
#pragma pack(pop)
	//처리용 패킷
	struct PPPacketForProcess {
		SOCKET m_socketSession;				//패킷을 보내는 세션의 소켓
		PPSendMode m_SendMode;				//PPSender에서 전송방법을 결정하기 위한 열거형 변수
		PPPacket m_Packet;					//실제 보낼 패킷
	};
}