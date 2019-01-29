#include "stdafx.h"
#include "TCPIOCPServer.h"


PP::TCPIOCPServer::TCPIOCPServer()
{
}


PP::TCPIOCPServer::~TCPIOCPServer()
{
}

int PP::TCPIOCPServer::Run()
{
	std::cout << "PP::TCPIOCPServer::Run()" << std::endl;
	
	return 0;
}

int PP::TCPIOCPServer::SetPort(const short * sPort)
{
	return 0;
}

int PP::TCPIOCPServer::Startup()
{
	LaunchThread(5);
	return 0;
}
