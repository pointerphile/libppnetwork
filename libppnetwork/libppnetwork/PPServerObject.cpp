#include "PPServerObject.h"
bool PPServerObject::m_isShutdown = false;

PPServerObject::PPServerObject() {}
PPServerObject::~PPServerObject() {}

int PPServerObject::LaunchThread()
{
	m_threadThis = std::thread(&PPServerObject::Run, this);
	m_threadID = m_threadThis.get_id();
	m_threadThis.detach();
	return 0;
}

void PPServerObject::DisplayError(const TCHAR * strParam)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		nullptr
	);
	std::wcout << strParam << _TEXT(" : ") << (LPCTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
}
