#pragma once
#include <iostream>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <WinSock2.h>
#include "PPProtocol.h"
#include "PPOVERLAPPED.h"

#pragma comment(lib, "ws2_32")

#ifdef LIBPPNETWORK_EXPORTS
#define LIBPPNETWORK_API __declspec(dllexport)
#else
#define LIBPPNETWORK_API __declspec(dllimport)
#endif

constexpr auto ASYNCFLAG_RECV = 0x00;
constexpr auto ASYNCFLAG_SEND = 0x01;

#define TEST