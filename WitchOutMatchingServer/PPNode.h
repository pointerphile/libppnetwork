#pragma once
#include "../libppnetwork/PPInclude.h"

namespace PP {
	class PPNode {
	public:
		PPNode();
		virtual ~PPNode();
	public:
		virtual int Run() = 0;
	};

}