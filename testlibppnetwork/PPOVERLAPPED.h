#pragma once
namespace PP {
	struct PPOVERLAPPED : OVERLAPPED {
		unsigned short dwFlag;
	};
}
constexpr auto ASYNCFLAG_RECV = 0;
constexpr auto ASYNCFLAG_SEND = 1;