#pragma once
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <WinSock2.h>
#include <Tchar.h>
#include "PPProtocol.h"
#include "PPSingleton.h"

#pragma comment(lib, "ws2_32")

#define BUFFER_SIZE 2048
#if defined(UNICODE) || defined(_UNICODE)
#define TCOUT wcout
#else
#define TCOUT cout
#endif