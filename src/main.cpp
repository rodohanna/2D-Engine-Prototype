#include "SDLWrapper.h"
#include "EventBus.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Events.h"
#include "GameTypes.h"
#include "Player.h"
#include <algorithm>

int main()
{
    // initialize SDL
    SDLWrapper sdl;
    if (!sdl.initializeSDL(800, 640))
    {
        printf("SDL failed to initialze.\n");
        return 1;
    }
    // initialize systems
    EventBus eventBus;
    RenderSystem renderSystem(sdl.renderer, &eventBus);
    InputSystem inputSystem(&eventBus);
    Rect box = {(800 - 100) / 2, (640 - 100) / 2, 100, 100};
    Color color = {0x11, 0x11, 0xFF, 0xFF};
    Player player(&eventBus, box, color);
    // start game loop
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    if (SDL_GetDisplayMode(0, 0, &mode) != 0)
    {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    int64_t performanceFrequency = SDL_GetPerformanceFrequency();
    bool refreshRateSpecified = false;
    double ts = 1.f / 60.f;
    int64_t desiredFrameTime = performanceFrequency / 60;
    if (mode.refresh_rate > 0 && sdl.vsync)
    {
        ts = 1.f / (double)mode.refresh_rate;
        desiredFrameTime = performanceFrequency / mode.refresh_rate;
        refreshRateSpecified = true;
    }
    printf("Initializing with ts: %f\n", ts);
    printf("Refresh rate: %d\n", mode.refresh_rate);
    int64_t prevFrameTime = SDL_GetPerformanceCounter();
    while (!inputSystem.quit)
    {
        int64_t currentFrameTime = SDL_GetPerformanceCounter();
        int64_t deltaTime = currentFrameTime - prevFrameTime;
        if (!refreshRateSpecified && deltaTime < desiredFrameTime)
        {
            int64_t millis = (double)(((desiredFrameTime - deltaTime)) * 1000) / performanceFrequency;
            if (millis >= 10)
            {
                SDL_Delay(4);
            }
            continue;
        }
        prevFrameTime = currentFrameTime;

        //handle unexpected timer anomalies (overflow, extra slow frames, etc)
        if (deltaTime > desiredFrameTime * 8)
        {
            deltaTime = desiredFrameTime;
        }
        if (deltaTime < 0)
        {
            deltaTime = 0;
        }

        eventBus.clearInputEvents();
        inputSystem.collectInputEvents();
        eventBus.notifyInputEventSubscribers();

        eventBus.clearRenderEvents();
        player.update(ts);
        eventBus.notifyRenderEventSubscribers(1.0);
    }
    return 0;
}