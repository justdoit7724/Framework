#pragma once

#include "DX_info.h"

template<class T>
class Singleton {

public:

	static T* Instance() {
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

	void Release()
	{
		delete instance;
		instance = nullptr;
	}

protected:
	Singleton() {}
	~Singleton() {}


private:
	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};
	static T* instance;
};


template<class T>
T* Singleton<T>::instance = nullptr;