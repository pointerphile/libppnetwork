#pragma once
#include "PPServerObject.h"
#include "PPInclude.h"
class PPSessionManager : public PPServerObject, public PPSingleton<PPSessionManager>
{
private:
	std::map<SOCKET, PPSession> m_mapSession;
	friend class PPSingleton<PPSessionManager>;
public:
	PPSessionManager();
	virtual ~PPSessionManager();
public:
	virtual int Init() { return 0; };
	virtual int Run() { return 0; };
	virtual int Release() { return 0; };
public:
	bool push_back(SOCKET socket, PPSession session);
	std::map<SOCKET, PPSession>::iterator begin();
	std::map<SOCKET, PPSession>::iterator end();
	std::map<SOCKET, PPSession>::iterator find(SOCKET socket);
	bool erase(SOCKET socket);
	bool clear();
	size_t size();
};

