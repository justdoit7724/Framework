#pragma once


#define SET_SINGLETON(classname)\
public:\
	~classname() {}\
	static classname& Instance() {\
		static classname instance;\
		return instance;\
	}\
	classname(classname const&)=delete;\
	classname& operator=(classname const&) =delete;\
private:\
	classname();

#define SET_SINGLETON_CPP(classname, func)\
classname::classname()\
{\
	classname::func();\
}
	