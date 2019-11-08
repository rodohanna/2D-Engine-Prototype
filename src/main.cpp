#include "SDLWrapper.h"
#include "EventBus.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Events.h"
#include "GameTypes.h"
#include "Player.h"
#include <algorithm>

double SDL_GetSecondsElapsed(int64_t old_counter, int64_t current_counter)
{
    return ((double)(current_counter - old_counter) / (double)(SDL_GetPerformanceFrequency()));
}

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
    EventBus event_bus;
    RenderSystem render_system(sdl.renderer, &event_bus);
    InputSystem input_system(&event_bus);
    Rect box = {(800 - 100) / 2, (640 - 100) / 2, 100, 100};
    Color color = {0x11, 0x11, 0xFF, 0xFF};
    Player player(&event_bus, box, color);
    // start game loop
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    if (SDL_GetDisplayMode(0, 0, &mode) != 0)
    {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    double ts = 1.f / 60.f;
    if (mode.refresh_rate > 0 && sdl.vsync)
    {
        ts = 1.f / (double)mode.refresh_rate;
    }
    ts = 1.f / 60.f;
    printf("Initializing with ts: %f\n", ts);
    printf("Refresh rate: %d\n", mode.refresh_rate);
    int64_t last_counter = SDL_GetPerformanceCounter();
    while (!input_system.quit)
    {

        event_bus.clearInputEvents();
        input_system.collectInputEvents();
        event_bus.notifyInputEventSubscribers();

        event_bus.clearRenderEvents();
        player.update(ts);

        if (SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) < ts)
        {
            int64_t TimeToSleep = ((ts - SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter())) * 1000) - 1;
            if (TimeToSleep > 0)
            {
                SDL_Delay(TimeToSleep);
            }
            while (SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) < ts)
            {
                // Waiting...
            }
        }
        int64_t end_counter = SDL_GetPerformanceCounter();

        event_bus.notifyRenderEventSubscribers(1.0);

        last_counter = end_counter;
    }
    return 0;
}