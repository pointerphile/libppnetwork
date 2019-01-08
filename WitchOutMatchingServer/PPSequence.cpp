#include "PPSequence.h"

PP::PPSequence::PPSequence() {}
PP::PPSequence::~PPSequence() {}

int PP::PPSequence::Run() {
	for (PP::PPNode* pNode : GetChildren()) {
		pNode->Run();
	}
	return 0;
}
