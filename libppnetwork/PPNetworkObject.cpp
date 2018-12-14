#include "PPNetworkObject.h"

PP::PPNetworkObject::PPNetworkObject() {}
PP::PPNetworkObject::~PPNetworkObject() {}

int PP::PPNetworkObject::LaunchThread() {
	m_threadThis = std::thread(&PPNetworkObject::Run, this);
	m_threadThis.detach();
	return 0;
}

int PP::PPNetworkObject::DisplayError(const wchar_t * wcharString) {
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
	std::wcout << wcharString << L" : " << (LPWSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);

	return 0;
}

int PP::PPNetworkObject::MessageBoxError(const wchar_t * wcharString) {
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
	MessageBoxW(nullptr, (LPCWSTR)lpMsgBuf, wcharString, 0);
	LocalFree(lpMsgBuf);

	return 0;
}
