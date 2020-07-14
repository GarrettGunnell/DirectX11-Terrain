#pragma once

#include <windows.h>

class Timer {
public:
	Timer();

	bool Initialize();
	void Frame();

	double GetTime();

	void StartTimer();
	void StopTimer();
	int GetTiming();

private:
	double frequency;
	double frameTime;
	INT64 startTime, beginTime, endTime;
};