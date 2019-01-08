#pragma once
#include "PPNode.h"

namespace PP {
	class PPGroup :	public PPNode {
	private:
		int m_iMinimumPlayer;
		int m_iMaximumPlayer;
		std::list<SOCKET> listSession;
	public:
		PPGroup();
		virtual ~PPGroup();
	};
}

