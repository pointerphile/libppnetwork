#include "stdafx.h"
#include "NetworkObject.h"


PP::NetworkObject::NetworkObject()
{
}


PP::NetworkObject::~NetworkObject()
{
}

const int PP::NetworkObject::LaunchThread(const unsigned short& usNumThreads)
{
	for (unsigned short usCounter = 0; usCounter < usNumThreads; ++usCounter) {
		m_ThreadPool.emplace_back(std::thread(&NetworkObject::Run, this));
		m_ThreadPool.back().detach();
	}
	return 0;
}
