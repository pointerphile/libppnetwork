#include "PPSession.h"

PP::PPSession::PPSession() {}
PP::PPSession::~PPSession() {
	if (m_socketSession != INVALID_SOCKET) {
		closesocket(m_socketSession);
	}
}