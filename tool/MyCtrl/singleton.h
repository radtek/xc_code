#ifndef _SINGLETON_h
#define _SINGLETON_h

template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

};

#endif