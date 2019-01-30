#include "pch.h"

int main()
{
	std::locale::global(std::locale(""));
	PP::TCPIOCPServer Server;
	Server.Startup();

	Sleep(2000);

	return 0;
}