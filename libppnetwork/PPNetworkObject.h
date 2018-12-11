#pragma once
#include "PPInclude.h"

namespace PP {
	class PPNetworkObject {
	private:
		std::thread m_threadThis;
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
	};
}