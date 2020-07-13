#include "Timer.h"
#include <stdio.h>

Timer::Timer() {
	frequency = frameTime = 0;
	startTime = beginTime = endTime = 0;
}

bool Timer::Initialize() {
	INT64 freq;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	if (freq == 0)
		return false;

	frequency = (double)freq;

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	return true;
}

void Timer::Frame() {
	INT64 currentTime, elapsedTicks;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	elapsedTicks = currentTime - startTime;
	frameTime = (double)elapsedTicks / frequency;

	startTime = currentTime;
}

double Timer::GetTime() {
	return frameTime;
}

void Timer::StartTimer() {
	QueryPerformanceCounter((LARGE_INTEGER*)&beginTime);
}


void Timer::StopTimer() {
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);
}

int Timer::GetTiming() {
	float elapsedTicks;
	INT64 frequency;
	float milliseconds;

	elapsedTicks = (float)(endTime - beginTime);

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	milliseconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliseconds;
}