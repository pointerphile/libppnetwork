#pragma once
#include "PPInclude.h"
#include "PPSingleton.h"
#include "PPSender.h"

namespace PP {
	//서버와의 접속을 종료시키거나 강제종료된 세션을 관리하는 클래스입니다.
	class PPItachHunter : public PP::PPSingleton<PPItachHunter> {
	private:
		friend class PPSingleton<PPItachHunter>;
	public:
		std::list<SOCKET> m_listItach;
		std::mutex m_mutex;
	public:
		PPItachHunter();
		~PPItachHunter();
	public:
		size_t size();
		bool empty();
		void push_back(SOCKET socketSession);
		void pop_front();
		SOCKET front();
	};
}