#include "Timer.h"

Timer::Timer() {
	frequency = frameTime = 0;
}

bool Timer::Initialize() {
	LARGE_INTEGER freq;

	if (QueryPerformanceFrequency(&freq) == false)
		return false;

	frequency = (double)freq.QuadPart;

	QueryPerformanceCounter(&startTime);

	return true;
}

void Timer::Frame() {
	LARGE_INTEGER currentTime, elapsedTicks;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	elapsedTicks = currentTime = startTime;
	frameTime = (double)elapsedTicks.QuadPart / frequency;
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

	elapsedTicks = (double)(endTime.QuadPart - beginTime.QuadPart);

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	milliseconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliseconds;
}