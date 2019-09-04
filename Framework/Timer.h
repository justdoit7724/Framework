#pragma once
#include <chrono>
class Timer
{
public:
	static void Init();
	static void Update();
	static float Elapsed();
	static unsigned int FPS() {
		return fps;
	}
	static float SPF() {
		return spf;
	}

private:
	Timer() {}
	static std::chrono::time_point<std::chrono::steady_clock> prev;
	static std::chrono::time_point<std::chrono::steady_clock> first;

	static float spf;
	static int fps;
};

