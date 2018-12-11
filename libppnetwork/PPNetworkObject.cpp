#include "PPNetworkObject.h"

PP::PPNetworkObject::PPNetworkObject() {}
PP::PPNetworkObject::~PPNetworkObject() {}

int PP::PPNetworkObject::LaunchThread() {
	m_threadThis = std::thread(&PPNetworkObject::Run, this);
	m_threadThis.detach();
	return 0;
}

int PP::PPNetworkObject::DisplayError(const wchar_t * wcharString) {
	return 0;
}
