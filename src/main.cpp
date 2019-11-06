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
    FRect box = {(800 - 100) / 2, (640 - 100) / 2, 100, 100};
    Color color = {0x11, 0x11, 0xFF, 0xFF};
    Player player(&eventBus, box, color);
    // start game loop
    double updateRate = 60;
    int64_t desiredFrameTime = SDL_GetPerformanceFrequency() / updateRate;
    int64_t prevFrameTime = SDL_GetPerformanceCounter();
    int64_t frameAccumulator = 0;
    while (!inputSystem.quit)
    {
        int64_t currentFrameTime = SDL_GetPerformanceCounter();
        int64_t deltaTime = currentFrameTime - prevFrameTime;
        if (deltaTime < desiredFrameTime)
        {

            int64_t millis = (double)(((desiredFrameTime - deltaTime)) * 1000) / SDL_GetPerformanceFrequency();
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

        frameAccumulator += deltaTime;

        /** 
         * GL Structure:
         * 
         * Collect Input Events
         * Notify Input Event Subscribers
         * 
         * Update Game Objects
         * Notify Game Object Subscribers
         * 
         * Notify Render Event Subscribers
         **/

        inputSystem.collectInputEvents();
        eventBus.notifyInputEventSubscribers();
        eventBus.clearInputEvents();
        int i = 0;
        while (frameAccumulator >= desiredFrameTime)
        {
            i++;
            eventBus.clearRenderEvents();
            player.update();
            frameAccumulator -= desiredFrameTime;
        }
        printf("%d", i);
        eventBus.notifyRenderEventSubscribers(1.0);
    }
    return 0;
}