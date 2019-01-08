#pragma once
#include "PPCompositeNode.h"

namespace PP {
	class PPSequence : public PPCompositeNode
	{
	public:
		PPSequence();
		virtual ~PPSequence();
	public:
		virtual int Run() override;
	};
}