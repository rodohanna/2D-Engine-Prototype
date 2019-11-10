#ifndef RENDERSYSTEM_h_
#define RENDERSYSTEM_h_

#include "SDLWrapper.h"
#include "EventBus.h"

struct RenderSystem : IRenderEventSubscriber
{
    RenderSystem(SDL_Renderer *, EventBus *);
    ~RenderSystem();
    void handle_render_events(const RenderEvent *, size_t, double);
    void render();
    void update();
    SDL_Renderer *renderer;
    EventBus *event_bus;
};

#endif