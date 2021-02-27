# Speedrun Timer Q3

Code for managing a speedrun timer for **id Tech 3** engine games. Mostly for
the Jedi Knight games by Raven Software, but could work fine for other
**id Tech 3** games too.

## Usage

Include this repository as
[submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) in your project
and add `timer.cpp` and `timer_helper.cpp` to your build system. Then a header
file called `speedrun_timer_includes.h` has to be created directly outside the
submodule. It needs to declare the functions

```c
void SpeedrunTimerPrint(bool printNotify,
                        const char *levelTimeString,
                        const char *totalTimeString);
int SpeedrunTimerGetSystemMilliseconds();
```

and implement them so that the first one prints the level and total time to the
console in an appropriate way and the second one returns the engine'ss
milliseconds timer, e.g. `Sys_Milliseconds` in most **id Tech 3** games.
