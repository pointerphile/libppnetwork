#pragma once
#include "PPNode.h"

namespace PP {
	class PPCompositeNode : public PP::PPNode {
	private:
		std::list<PP::PPNode*> listChildren;
	public:
		PPCompositeNode();
		virtual ~PPCompositeNode();
	public :
		void AddChild(PP::PPNode* child);
		const std::list<PP::PPNode*>& GetChildren() const;
	};
}