#pragma once
#include "PPNetworkObject.h"

namespace PP {
	class PPPacketPool{
	public:
		std::list<PPPacketForProcess> m_listPacket;
	public:
		PPPacketPool();
		virtual ~PPPacketPool();
	public:
		LIBPPNETWORK_API PPPacketForProcess& front();
		LIBPPNETWORK_API void push_back(PPPacketForProcess packet);
		LIBPPNETWORK_API void pop_front();
		LIBPPNETWORK_API size_t size();
		LIBPPNETWORK_API bool empty();
		LIBPPNETWORK_API void clear();
	};
}