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
    int64_t desiredFrameRate = SDL_GetPerformanceFrequency() / updateRate;
    int64_t prevFrameTime = SDL_GetPerformanceCounter();
    int64_t frameAccumulator = 0;
    while (!inputSystem.quit)
    {
        int64_t currentFrameTime = SDL_GetPerformanceCounter();
        int64_t deltaTime = currentFrameTime - prevFrameTime;
        prevFrameTime = currentFrameTime;

        //handle unexpected timer anomalies (overflow, extra slow frames, etc)
        if (deltaTime > desiredFrameRate * 8)
        {
            deltaTime = desiredFrameRate;
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

        while (frameAccumulator >= desiredFrameRate)
        {
            eventBus.clearRenderEvents();
            player.update();
            frameAccumulator -= desiredFrameRate;
        }

        double alpha = (double)frameAccumulator / (double)desiredFrameRate;

        eventBus.notifyRenderEventSubscribers(alpha);
    }
    return 0;
}