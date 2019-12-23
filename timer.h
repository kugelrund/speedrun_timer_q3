#pragma once


void SpeedrunResetTimer();
void SpeedrunUpdateTimer();
void SpeedrunUnpauseTimer();
bool SpeedrunPauseTimer();
void SpeedrunLevelFinished();
void SpeedrunRunFinished();
int SpeedrunGetTotalTimeMilliseconds();
int SpeedrunGetLevelTimeMilliseconds();
