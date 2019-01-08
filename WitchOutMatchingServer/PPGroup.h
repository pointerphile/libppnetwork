#pragma once
#include "PPNode.h"

namespace PP {
	class PPGroup :	public PPNode {
	private:
		int m_iMinimumPlayer;
	public:
		PPGroup();
		virtual ~PPGroup();
	};
}

