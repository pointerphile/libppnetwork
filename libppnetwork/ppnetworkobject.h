#pragma once
#include "ppinclude.h"

namespace PP {
	class PPNetworkObject
	{
	public:
		PPNetworkObject();
		virtual ~PPNetworkObject();
	public:
		virtual int Init() = 0;
		virtual int Run() = 0;
		virtual int Release() = 0;
	public:
		int LaunchThread();
		int DisplayError(const wchar_t* wcharString);
	};
}