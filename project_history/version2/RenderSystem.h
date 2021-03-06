#ifndef RENDERSYSTEM_h_
#define RENDERSYSTEM_h_

#include "SDLWrapper.h"
#include "EventBus.h"
#include "Assets.h"
#include <vector>
#include <memory>

struct RenderSystem : IRenderEventSubscriber, IInputEventSubscriber
{
    RenderSystem(SDL_Renderer *, EventBus *);
    ~RenderSystem();
    void handle_render_events(const Events::RenderEvent *, size_t);
    void handle_input_events(const Events::InputEvent *, size_t);
    void render(const Events::RenderEvent *, size_t);
    void update_cameras(double w, double h);
    void update_mouse_positions();
    double gui_render_scale, world_render_scale;
    SDL_Renderer *renderer;
    EventBus *event_bus;
    std::vector<std::unique_ptr<Texture>> *texture_table;
};

#endif