#pragma once
#include "Singleton.h"
#include <chrono>

#define SGL_TIMER Timer::Instance()

class Timer
{
	SET_SINGLETON(Timer)

public:
	void Update();
	float Elapsed();
	unsigned int FPS() {
		return fps;
	}
	float SPF() {
		return spf;
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> prev;
	std::chrono::time_point<std::chrono::steady_clock> first;

	float spf;
	int fps;

	void Init();
};

