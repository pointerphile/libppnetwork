#pragma once
template<class T> class PPSingleton
{
public:
	static T& GetInstance()
	{
		static T SingleInstance;
		return SingleInstance;
	}
};