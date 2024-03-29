#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;

SET_SINGLETON_CPP(Timer, Init)

float fpsSec = 0;
void Timer::Update()
{
	auto curTime = high_resolution_clock::now();
	spf = duration<double, std::deca>(curTime - prev).count() * 10;
	prev = curTime;

	fpsSec += spf;

	if (fpsSec < 0.5f) // update fps per 0.5 sec
		return;
	fpsSec = 0;

	fps = 1.0f / spf;
}

float Timer::Elapsed()
{
	return duration<double, std::deca>(high_resolution_clock::now() - first).count() * 10;
}

void Timer::Init()
{

	fps = 0;
	spf = 0;
	prev = high_resolution_clock::now();
	first = high_resolution_clock::now();
}

