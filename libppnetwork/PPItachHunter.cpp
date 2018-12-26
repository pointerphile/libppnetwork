#include "PPItachHunter.h"

PP::PPItachHunter::PPItachHunter() {}
PP::PPItachHunter::~PPItachHunter() {}

size_t PP::PPItachHunter::size() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_listItach.size();
}

bool PP::PPItachHunter::empty() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_listItach.empty();
}

void PP::PPItachHunter::push_back(SOCKET socketSession) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_listItach.push_back(socketSession);
}

void PP::PPItachHunter::pop_front() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_listItach.pop_front();
}

SOCKET PP::PPItachHunter::front() {
	//std::lock_guard<std::mutex> lock(m_mutex);
	return m_listItach.front();
}
