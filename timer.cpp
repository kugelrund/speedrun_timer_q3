#include "../../server/exe_headers.h"
#include "timer.h"
#include "timer_helper.h"
#include <string>


int lastTimestamp = 0;
int storedTotalTime = 0;
int storedLevelTime = 0;
bool paused = true;

struct ExternVisibleInfo {
	int currentTotalTime = 0;
	int currentLevelTime = 0;
	bool isRunFinished = false;
};
volatile ExternVisibleInfo info;

void SpeedrunResetTimer()
{
	lastTimestamp = 0;
	storedTotalTime = 0;
	storedLevelTime = 0;
	info.currentTotalTime = 0;
	info.currentLevelTime = 0;
	info.isRunFinished = false;
	paused = true;
}

void SpeedrunUpdateTimer()
{
	if (paused)
	{
		return;
	}

	const int currentTimestamp = Sys_Milliseconds();
	info.currentTotalTime = storedTotalTime + (currentTimestamp - lastTimestamp);
	info.currentLevelTime = storedLevelTime + (currentTimestamp - lastTimestamp);
}

void SpeedrunUnpauseTimer()
{
	if (paused)
	{
		paused = false;
		lastTimestamp = Sys_Milliseconds();
	}
}

void SpeedrunStoreCurrentTime()
{
	const int currentTimestamp = Sys_Milliseconds();
	storedTotalTime += (currentTimestamp - lastTimestamp);
	storedLevelTime += (currentTimestamp - lastTimestamp);
	info.currentTotalTime = storedTotalTime;
	info.currentLevelTime = storedLevelTime;
	lastTimestamp = currentTimestamp;
}

bool SpeedrunPauseTimer()
{
	if (paused)
	{
		return true;
	}

	SpeedrunStoreCurrentTime();
	paused = true;
	return false;
}

void SpeedrunLevelFinished()
{
	if (!paused)
	{
		SpeedrunStoreCurrentTime();
	}

	constexpr int printAccuracy = 3;
	std::string timeString = GetTimeStringFromMilliseconds(info.currentLevelTime,
	                                                       printAccuracy);
	Com_Printf(S_COLOR_RED "=========================\n"
	           S_COLOR_GREEN "Level time: " S_COLOR_WHITE "%s\n",
	           timeString.c_str());
	if (info.currentLevelTime != info.currentTotalTime)
	{
		timeString = GetTimeStringFromMilliseconds(info.currentTotalTime,
		                                           printAccuracy);
		Com_Printf(S_COLOR_GREEN "Total time: " S_COLOR_WHITE "%s\n",
		           timeString.c_str());
	}
	Com_Printf(S_COLOR_RED "=========================\n");

	storedLevelTime = 0;
}

void SpeedrunRunFinished()
{
	SpeedrunPauseTimer();
	SpeedrunLevelFinished();
	info.isRunFinished = true;
}

int SpeedrunGetTotalTimeMilliseconds()
{
	return info.currentTotalTime;
}

int SpeedrunGetLevelTimeMilliseconds()
{
	return info.currentLevelTime;
}
