#pragma once
#include "PPInclude.h"
#include "PPSession.h"
static std::mutex m_mutex;
namespace PP {
	class PPNetworkObject {
	private:
		std::thread m_threadThis;
	protected:
		
#ifdef _DEBUG
		wchar_t m_wcharVersion[44] = L"libppnetwork [Version 0.1.0007] Debug Mode\n";
#else
		wchar_t m_wcharVersion[33] = L"libppnetwork [Version 0.1.0007]\n";
#endif // DEBUG
		
	public:
		PPNetworkObject();
		virtual ~PPNetworkObject();
	public:
		virtual int Init() = 0;
		virtual int Run() = 0;
		virtual int Release() = 0;
	public:
		virtual int LaunchThread();
		virtual int DisplayError(const wchar_t* wcharString);
		virtual int MessageBoxError(const wchar_t* wcharString);
	};
}