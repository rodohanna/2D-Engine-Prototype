#include "RenderSystem.h"

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : renderer(r), eventBus(eB)
{
    this->eventBus->subscribeToRenderEvents(this);
}
RenderSystem::~RenderSystem()
{
    this->eventBus->subscribeToRenderEvents(this);
}
void RenderSystem::handleRenderEvents(const RenderEvent *renderEvents, size_t length)
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
            SDL_RenderDrawRect(this->renderer, &e.data.renderRectangleEvent.rect);
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