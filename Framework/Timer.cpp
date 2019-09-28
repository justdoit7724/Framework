#include "Timer.h"

using namespace std::chrono;

float fpsSec = 0;
void Timer::Update()
{
	auto curTime = high_resolution_clock::now();
	spf = duration<float, std::deca>(curTime - prev).count()*10.0f;
	prev = curTime;

	fpsSec += spf;

	if (fpsSec < 0.5f) // update fps per 0.5 sec
		return;
	fpsSec = 0;

	fps = 1.0f / spf;
}

float Timer::Elapsed()
{
	return duration<float, std::deca>(high_resolution_clock::now() - first).count()*10;
}

Timer::Timer()
{
	fps = 0;
	spf = 0;
	prev = high_resolution_clock::now();
	first = high_resolution_clock::now();
}

