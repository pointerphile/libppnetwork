#pragma once
#include <iostream>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <WinSock2.h>
#include "ppprotocol.h"

#pragma comment(lib, "ws2_32")

#ifdef LIBPPNETWORK_EXPORTS
#define LIBPPNETWORK_API __declspec(dllexport)
#else
#define LIBPPNETWORK_API __declspec(dllimport)
#endif