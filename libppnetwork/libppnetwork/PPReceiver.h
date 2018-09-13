#pragma once
#include "PPServerObject.h"
class PPReceiver :
	public PPServerObject
{
public:
	PPReceiver();
	virtual ~PPReceiver();
public:
	int Init();
	int Run();
	int Release();
public:
	int Receive(PPSession Session, DWORD dwTransferred);
};

