#pragma once


void SpeedrunResetTimer();
void SpeedrunUpdateTimer();
void SpeedrunUnpauseTimer(int priority = 0);
void SpeedrunPauseTimer(int priority = 0);
void SpeedrunLevelFinished();
void SpeedrunRunFinished();
int SpeedrunGetTotalTimeMilliseconds();
int SpeedrunGetLevelTimeMilliseconds();
