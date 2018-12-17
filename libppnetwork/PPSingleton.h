#pragma once
namespace PP {
	template<class T> class PPSingleton
	{
	public:
		LIBPPNETWORK_API static T& GetInstance() {
			static T SingleInstance;
			return SingleInstance;
		}
	};
}