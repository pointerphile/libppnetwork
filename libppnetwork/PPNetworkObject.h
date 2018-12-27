#pragma once
#include "PPInclude.h"
#include "PPSession.h"

namespace PP {
	class PPNetworkObject {
	private:
		std::thread m_threadThis;
	protected:
		wchar_t m_wcharVersion[33] = L"libppnetwork [Version 0.1.0004]\n";
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