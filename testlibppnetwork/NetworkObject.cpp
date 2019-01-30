#include "stdafx.h"
#include "NetworkObject.h"


PP::NetworkObject::NetworkObject()
{
}


PP::NetworkObject::~NetworkObject()
{
}

const int PP::NetworkObject::LaunchThread(const unsigned short& uiNumThreads)
{
	for (unsigned short usCounter = 0; usCounter < uiNumThreads; ++usCounter) {
		m_ThreadPool.emplace_back(std::thread(&NetworkObject::Run, this));
		m_ThreadPool.back().detach();
	}
	return 0;
}

void PP::NetworkObject::GetError(const std::wstring & wstring)
{
	std::wstring wstrOutput;
	LPVOID lpMsgBuf;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		0,//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0,
		nullptr
	);
	wstrOutput = wstrOutput + wstring + L": " + (LPWSTR)lpMsgBuf;
	std::wcout << wstrOutput.c_str();
	LocalFree(lpMsgBuf);
}
