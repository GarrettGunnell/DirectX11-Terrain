#pragma once

#include <windows.h>

class Timer {
public:
	Timer();

	bool Initialize();
	void Frame();

	float GetTime();

	void StartTimer();
	void StopTimer();
	int GetTiming();

private:
	float frequency;
	float frameTime;
	INT64 startTime, beginTime, endTime;
};