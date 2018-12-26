#pragma once
#include "PPInclude.h"
#include "PPSingleton.h"
#include "PPSender.h"

namespace PP {
	//�������� ������ �����Ű�ų� ��������� ������ �����ϴ� Ŭ�����Դϴ�.
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