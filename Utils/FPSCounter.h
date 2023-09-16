#pragma once

#include <chrono>
#include <thread>

inline long long nanoTime()
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}


struct FPSCounter {

	double start = nanoTime();
	double elapsed{}, curr{};
	unsigned int FPS = 0;
	
	void step() {

		curr = nanoTime();
		elapsed = (curr - start) / 1E9;
		start = curr;
		FPS = 1 / elapsed;

	}


};
