#include "Timer.h"

Timer makeTimer()
{
    Timer timer = {0, 0, false, false};
    return timer;
}
void timerStart(Timer &timer)
{
    timer.started = true;
    timer.paused = false;
    timer.startTicks = SDL_GetTicks();
    timer.pausedTicks = 0;
}
void timerStop(Timer &timer)
{
    timer.started = false;
    timer.paused = false;
    timer.startTicks = 0;
    timer.pausedTicks = 0;
}
void timerPause(Timer &timer)
{
    if (timer.started && !timer.paused)
    {
        timer.paused = true;
        timer.pausedTicks = SDL_GetTicks();
        timer.startTicks = 0;
    }
}
void timerUnpause(Timer &timer)
{
    if (timer.started && timer.paused)
    {
        timer.paused = false;
        timer.startTicks = SDL_GetTicks() - timer.pausedTicks;
        timer.pausedTicks = 0;
    }
}
Uint32 timerGetTicks(Timer &timer)
{
    if (timer.started)
    {
        if (timer.paused)
        {
            return timer.pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - timer.startTicks;
        }
    }
    return 0;
}