#include "timer.h"
#include "timer_helper.h"
#include "../speedrun_timer_includes.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>


int lastTimestamp = 0;
int storedTotalTime = 0;
int storedLevelTime = 0;

// Implements pausing with different priorities. For example, unpausing with
// priority 1 will not actually unpause, if it is currently also paused with
// priority 0. This allows handling overlapping pausing events of different
// priority. For example, if the start of loading a new map happens earlier
// than cutscene skipping being done, we can make "loading a new map" priority 0
// and "cutscene skipping" priority 1 and it will be handled properly.
class PausedState {
public:
	static constexpr int num_pause_flag_levels = 3;

	PausedState() {
		std::fill(pause_flags.begin(), pause_flags.end(), true);
	}

	void pause(int priority) {
		assert(priority >= 0 && priority < num_pause_flag_levels);
		std::fill(pause_flags.begin() + priority, pause_flags.end(), true);
	}

	void unpause(int priority) {
		assert(priority >= 0 && priority < num_pause_flag_levels);
		std::fill(pause_flags.begin() + priority, pause_flags.end(), false);
	}

	bool isPaused() const {
		return std::any_of(pause_flags.cbegin(), pause_flags.cend(),
		                   [](const auto flag) { return flag; });
	}

private:
	std::array<bool, num_pause_flag_levels> pause_flags;
};
PausedState pause_state{};


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
	pause_state.pause(0);
}

void SpeedrunUpdateTimer()
{
	if (pause_state.isPaused())
	{
		return;
	}

	const int currentTimestamp = SpeedrunTimerGetSystemMilliseconds();
	info.currentTotalTime = storedTotalTime + (currentTimestamp - lastTimestamp);
	info.currentLevelTime = storedLevelTime + (currentTimestamp - lastTimestamp);
}

void SpeedrunUnpauseTimer(int priority)
{
	const bool was_paused = pause_state.isPaused();
	pause_state.unpause(priority);
	if (was_paused && !pause_state.isPaused())
	{
		lastTimestamp = SpeedrunTimerGetSystemMilliseconds();
	}
}

void SpeedrunStoreCurrentTime()
{
	const int currentTimestamp = SpeedrunTimerGetSystemMilliseconds();
	storedTotalTime += (currentTimestamp - lastTimestamp);
	storedLevelTime += (currentTimestamp - lastTimestamp);
	info.currentTotalTime = storedTotalTime;
	info.currentLevelTime = storedLevelTime;
	lastTimestamp = currentTimestamp;
}

void SpeedrunPauseTimer(int priority)
{
	if (!pause_state.isPaused())
	{
		SpeedrunStoreCurrentTime();
	}
	pause_state.pause(priority);
}

void SpeedrunLevelFinished(bool printNotify)
{
	if (!pause_state.isPaused())
	{
		SpeedrunStoreCurrentTime();
	}

	constexpr int printAccuracy = 3;
	const std::string levelTimeString = GetTimeStringFromMilliseconds(
		info.currentLevelTime, printAccuracy);
	if (info.currentLevelTime != info.currentTotalTime)
	{
		const std::string totalTimeString = GetTimeStringFromMilliseconds(
			info.currentTotalTime, printAccuracy);
		SpeedrunTimerPrint(printNotify, levelTimeString.c_str(), totalTimeString.c_str());
	} else {
		SpeedrunTimerPrint(printNotify, levelTimeString.c_str(), nullptr);
	}
	storedLevelTime = 0;
}

void SpeedrunRunFinished(bool printNotify)
{
	if (info.isRunFinished)
	{
		return;
	}

	SpeedrunPauseTimer();
	SpeedrunLevelFinished(printNotify);
	info.isRunFinished = true;
}

bool SpeedrunIsRunFinished() {
	return info.isRunFinished;
}

int SpeedrunGetTotalTimeMilliseconds()
{
	return info.currentTotalTime;
}

int SpeedrunGetLevelTimeMilliseconds()
{
	return info.currentLevelTime;
}
