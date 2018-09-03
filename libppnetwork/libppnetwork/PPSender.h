#pragma once
#include "PPServerObject.h"
class PPSender : public PPServerObject {
public:
	PPSender();
	virtual ~PPSender();
public:
	int Init();
	int Run();
	int Release();
public:
	int Send();
	int Broadcast();
};

