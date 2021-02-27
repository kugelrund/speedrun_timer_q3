#pragma once


void SpeedrunResetTimer();
void SpeedrunUpdateTimer();
void SpeedrunUnpauseTimer(int priority = 0);
void SpeedrunPauseTimer(int priority = 0);
void SpeedrunLevelFinished();
void SpeedrunRunFinished();
bool SpeedrunIsRunFinished();
int SpeedrunGetTotalTimeMilliseconds();
int SpeedrunGetLevelTimeMilliseconds();
