#pragma once
#include "stdafx.h"

namespace PP {
	class PacketMgr
	{
	private:
		std::vector<int> m_vectorPacket;
	public:
		PacketMgr();
		~PacketMgr();
	};
}