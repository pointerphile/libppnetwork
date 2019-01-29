// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// Requires ws2_32.lib
#include <WinSock2.h>
// Requires Iphlpapi.lib
//#include <iphlpapi.h>
// reference additional headers your program requires here
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
//C++ 컨테이너
#include <string>
#include <vector>
#include <list>
#include <map>

#ifdef TESTLIBPPNETWORK_EXPORTS
#define LIBPPNETWORK_API __declspec(dllexport)
#else
#define LIBPPNETWORK_API __declspec(dllimport)
#endif