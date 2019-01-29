#pragma once
#include "stdafx.h"

namespace PP {
	class NetworkObject
	{
	private:
		std::vector<std::thread> m_ThreadPool;
	public:
		NetworkObject();
		virtual ~NetworkObject();
	public:
		virtual int Run() = 0;
	public:
		const int LaunchThread(const unsigned short& usNumThreads);
	};
}