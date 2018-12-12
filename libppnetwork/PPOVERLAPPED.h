#pragma once
#include <WinSock2.h>
struct PPOVERLAPPED : OVERLAPPED {
	DWORD dwFlag;
};