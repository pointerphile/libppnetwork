#include "PPCompositeNode.h"

PP::PPCompositeNode::PPCompositeNode() {}
PP::PPCompositeNode::~PPCompositeNode() {}

void PP::PPCompositeNode::AddChild(PPNode * child) {
	listChildren.emplace_back(child);
}

const std::list<PP::PPNode*>& PP::PPCompositeNode::GetChildren() const {
	return listChildren;
}
