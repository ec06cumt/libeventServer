
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "..\lock\LockObject.h"
//#include "lock\GuardLock.h"
#include <memory>

using namespace std;

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		CLockObject lockObject;
		lockObject.Lock();
		if (NULL == instance.get())
		{
			instance.reset( new (T));
		}
		lockObject.Unlock();
		return *instance;
	}

	friend T;
protected:
	Singleton(){};
	~Singleton(){};
	
	static std::auto_ptr<T>  instance;
private:
	Singleton(const Singleton&);
	Singleton& operator =(const Singleton&);
	

};

template <typename T>
std::auto_ptr<T>  Singleton<T>::instance ;

#endif //_SINGLETON_H_