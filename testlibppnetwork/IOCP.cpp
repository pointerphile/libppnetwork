#include "stdafx.h"
#include "IOCP.h"


PP::IOCP::IOCP()
{
}


PP::IOCP::~IOCP()
{
}

int PP::IOCP::Run()
{
	return 0;
}

int PP::IOCP::SetProcessPacket(const std::function<int()>& fnParam)
{
	m_fnProcessPacket = fnParam;
	return 0;
}
