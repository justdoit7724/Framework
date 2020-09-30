#pragma once

#include <chrono>

namespace DX {
	class Timer
	{
	public:
		Timer();
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
	};
}

