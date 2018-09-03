#pragma once
#include "PPServerObject.h"
#include "PPIOCP.h"

class PPConnector : public PPServerObject {
private:
	std::string m_strAddress;
	short m_iPort;
public:
	PPConnector();
	virtual ~PPConnector();
	int Set(std::string strAddress, short iPort);
public:
	virtual int Init();
	virtual int Run();
	virtual int Release();
};

