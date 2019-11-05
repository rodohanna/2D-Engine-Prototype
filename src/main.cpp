#include "SDLWrapper.h"
#include "EventBus.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Events.h"
#include "GameTypes.h"
#include "Player.h"

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
    Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    Player player(&eventBus, box, color);
    // start game loop
    while (!inputSystem.quit)
    {
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
        player.update();
        eventBus.notifyRenderEventSubscribers();
        eventBus.clear();
    }
    return 0;
}