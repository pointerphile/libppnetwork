#pragma once
#include "PPNetworkObject.h"
#include "PPSingleton.h"
namespace PP {
	class PPSessionManager : public PPSingleton<PPSessionManager> {
	private:
		friend class PPSingleton<PPSessionManager>;
	public:
		std::map<SOCKET, PPSession> m_mapSession;
	public:
		PPSessionManager();
		virtual ~PPSessionManager();
	public:
		bool insert(SOCKET socket, PPSession session);
		bool erase(SOCKET socket);
		const std::map<SOCKET, PPSession>::iterator find(SOCKET socket);
	};
}