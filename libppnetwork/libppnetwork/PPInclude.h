#pragma once
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <WinSock2.h>
#include <Tchar.h>
#include "PPProtocol.h"
#include "PPSingleton.h"

#pragma comment(lib, "ws2_32")

constexpr auto ASYNCFLAG_RECV = 0x01;
constexpr auto ASYNCFLAG_SEND = 0x02;

struct PPOVERLAPPED : OVERLAPPED {
	DWORD dwFlag;
};

#if defined(UNICODE) || defined(_UNICODE)
#define TCOUT wcout
#else
#define TCOUT cout
#endif