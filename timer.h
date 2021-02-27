#pragma once


void SpeedrunResetTimer();
void SpeedrunUpdateTimer();
void SpeedrunUnpauseTimer(int priority = 0);
void SpeedrunPauseTimer(int priority = 0);
void SpeedrunLevelFinished(bool printNotify = false);
void SpeedrunRunFinished(bool printNotify = true);
bool SpeedrunIsRunFinished();
int SpeedrunGetTotalTimeMilliseconds();
int SpeedrunGetLevelTimeMilliseconds();
