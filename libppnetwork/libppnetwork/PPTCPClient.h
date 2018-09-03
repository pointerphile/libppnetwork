#pragma once
#include "PPServerObject.h"
class PPTCPClient : public PPServerObject
{
public:
	PPTCPClient();
	virtual ~PPTCPClient();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
};

