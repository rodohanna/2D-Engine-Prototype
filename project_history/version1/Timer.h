#ifndef TIMER_h_
#define TIMER_h_
#include <SDL2/SDL.h>

struct Timer
{
    Uint32 startTicks;
    Uint32 pausedTicks;
    bool paused;
    bool started;
};
Timer makeTimer();
void timerStart(Timer &timer);
void timerStop(Timer &timer);
void timerPause(Timer &timer);
void timerUnpause(Timer &timer);
Uint32 timerGetTicks(Timer &timer);

#endif