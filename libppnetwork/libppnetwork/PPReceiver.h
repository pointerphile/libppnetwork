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
	int Run(PPSession* pSession, DWORD dwTransferred);
	int Release();
};

