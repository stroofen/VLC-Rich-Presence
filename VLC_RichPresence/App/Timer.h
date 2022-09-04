#ifndef TIMER_H
#define TIMER_H

#include "../Global.h"

#ifdef SYSTEM_WINDOWS
#pragma once
#endif

#include <chrono>

typedef std::chrono::high_resolution_clock::time_point TimePoint;

class Timer {
public:
	Timer() {
		start = std::chrono::high_resolution_clock::now();
	}

	__inline f32 elapsed() const {
		return std::chrono::duration<f32>((std::chrono::high_resolution_clock::now() - start)).count();
	}

	__inline bool resetIfElapsed(f32 seconds) {
		f32 passed = elapsed();
		if (passed >= seconds) {
			start = std::chrono::high_resolution_clock::now();
			return true;
		}
		return false;
	}

	__inline bool hasElapsed(f32 seconds) const {
		return (elapsed() >= seconds);
	}

	__inline f32 reset() {
		f32 passed = elapsed();
		start = std::chrono::high_resolution_clock::now();
		return passed;
	}

private:
	TimePoint start;
};

#endif