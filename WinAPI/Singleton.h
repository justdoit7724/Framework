#pragma once

template<class T>
class Singleton {

public:
	~Singleton() {}

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


private:
	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};
	static T* instance;
};


template<class T>
T* Singleton<T>::instance = nullptr;