#pragma once
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <list>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include "PPProtocol.h"
#include "PPSingleton.h"

#pragma comment(lib, "ws2_32")

#define BUFFER_SIZE 2048