#pragma once
#include "PPServerObject.h"
#include "PPConnector.h"
#include "PPReceiver.h"
#include "PPSender.h"
class PPTCPIOCPClient : public PPServerObject
{
private:
	PPConnector Connector;
	PPSender Sender;
public:
	PPTCPIOCPClient();
	virtual ~PPTCPIOCPClient();
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
public:
	int Startup(std::string strAddress, short iPort);
	int Shutdown();
};

