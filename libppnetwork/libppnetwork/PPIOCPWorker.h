#pragma once
#include "PPServerObject.h"
#include "PPReceiver.h"
#include "PPSender.h"
class PPIOCPWorker : public PPServerObject {
private:
	PPReceiver Receiver;
	PPSender Sender;
public:
	PPIOCPWorker();
	virtual ~PPIOCPWorker();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
};

