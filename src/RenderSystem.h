#ifndef RENDERSYSTEM_h_
#define RENDERSYSTEM_h_

#include <SDL2/SDL.h>
#include "EventBus.h"

struct RenderSystem : IRenderEventSubscriber
{
    RenderSystem(SDL_Renderer *, EventBus *);
    ~RenderSystem();
    void handleRenderEvents(const RenderEvent *, size_t, double);
    void render();
    void update();
    SDL_Renderer *renderer;
    EventBus *eventBus;
};

#endif