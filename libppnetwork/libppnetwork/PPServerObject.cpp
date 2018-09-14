#include "PPServerObject.h"
bool PPServerObject::m_isShutdown = false;
std::mutex PPServerObject::m_mutexThis;

PPServerObject::PPServerObject() {}
PPServerObject::~PPServerObject() {}

int PPServerObject::LaunchThread()
{
	m_threadThis = std::thread(&PPServerObject::Run, this);
	m_threadID = m_threadThis.get_id();
	m_threadThis.detach();
	return 0;
}

void PPServerObject::DisplayError(const wchar_t * strParam)
{
	LPVOID lpMsgBuf;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0,
		nullptr
	);
	std::wcout << strParam << L" : " << (LPCWSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
}
