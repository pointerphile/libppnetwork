#pragma once
#include "PPNetworkObject.h"
#include "PPSingleton.h"
namespace PP {
	class PPSessionManager : public PPNetworkObject, public PPSingleton<PPSessionManager> {
	private:
		friend class PPSingleton<PPSessionManager>;
	public:
		std::map<SOCKET, PP::PPSession> m_mapSession;
		
	public:
		PPSessionManager();
		virtual ~PPSessionManager();
	public:
		int Init();
		int Run();
		int Release();
	public:
		bool insert(SOCKET socket, PPSession session);
		bool erase(SOCKET socket);
		void clear();
		std::map<SOCKET, PP::PPSession>::iterator begin();
		std::map<SOCKET, PP::PPSession>::iterator end();
		const std::map<SOCKET, PP::PPSession>::iterator find(SOCKET socket);
		size_t size();
	};
}