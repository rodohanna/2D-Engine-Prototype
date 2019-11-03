#include "SDLWrapper.h"
#include "EventBus.h"
#include "RenderSystem.h"
#include "Events.h"

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
    // prepare game loop
    bool quit = false;
    SDL_Event e;
    // start game loop
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        RenderEvent e;
        e.type = RENDER_RECTANGLE;
        e.data.renderRectangleEvent.rect = {(800 - 100) / 2, (640 - 100) / 2, 100, 100};
        e.data.renderRectangleEvent.color = {0xFF, 0xFF, 0xFF, 0xFF};
        RenderEvent e1;
        e1.type = RENDER_RECTANGLE;
        e1.data.renderRectangleEvent.rect = {0, (640 - 100) / 2, 100, 100};
        e1.data.renderRectangleEvent.color = {0xFF, 0x00, 0xFF, 0xFF};
        RenderEvent e2;
        e2.type = RENDER_RECTANGLE;
        e2.data.renderRectangleEvent.rect = {(800 - 100), (640 - 100) / 2, 100, 100};
        e2.data.renderRectangleEvent.color = {0xFF, 0xFF, 0x00, 0xFF};
        eventBus.publishRenderEvent(e);
        eventBus.publishRenderEvent(e1);
        eventBus.publishRenderEvent(e2);
        eventBus.notify();
        eventBus.clear();
    }
    return 0;
}