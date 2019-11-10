#include "RenderSystem.h"
#include <stdio.h>

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : renderer(r), event_bus(eB)
{
    this->event_bus->subscribe_to_render_Events(this);
}
RenderSystem::~RenderSystem()
{
    this->event_bus->subscribe_to_render_Events(this);
}
void RenderSystem::handle_render_events(const RenderEvent *renderEvents, size_t length, double alpha)
{
    SDL_SetRenderDrawColor(this->renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(this->renderer);
    for (size_t i = 0; i < length; ++i)
    {
        RenderEvent e = renderEvents[i];
        switch (e.type)
        {
        case RENDER_RECTANGLE:
        {
            SDL_SetRenderDrawColor(
                renderer,
                e.data.renderRectangleEvent.color.r,
                e.data.renderRectangleEvent.color.g,
                e.data.renderRectangleEvent.color.b,
                e.data.renderRectangleEvent.color.a);

            SDL_RenderFillRect(this->renderer, &e.data.renderRectangleEvent.box);
            break;
        }
        }
    }
    SDL_RenderPresent(renderer);
}
void RenderSystem::render()
{
    //
}
void RenderSystem::update()
{
}