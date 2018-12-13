#pragma once
#include <WinSock2.h>

namespace PP {
	struct PPOVERLAPPED : OVERLAPPED {
		DWORD dwFlag;
	};
}