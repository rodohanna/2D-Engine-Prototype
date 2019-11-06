#include "RenderSystem.h"

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : renderer(r), eventBus(eB)
{
    this->eventBus->subscribeToRenderEvents(this);
}
RenderSystem::~RenderSystem()
{
    this->eventBus->subscribeToRenderEvents(this);
}
void RenderSystem::handleRenderEvents(const RenderEvent *renderEvents, size_t length, double alpha)
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
            SDL_Rect interpolatedRect = {
                static_cast<int>(e.data.renderRectangleEvent.now.x),
                static_cast<int>(e.data.renderRectangleEvent.now.y),
                e.data.renderRectangleEvent.now.w,
                e.data.renderRectangleEvent.now.h};
            // if (e.data.renderRectangleEvent.now.x != e.data.renderRectangleEvent.prev.x)
            // {
            //     interpolatedRect.x = static_cast<int>(e.data.renderRectangleEvent.now.x * alpha + e.data.renderRectangleEvent.prev.x * (1.0 - alpha));
            // }
            // if (e.data.renderRectangleEvent.now.y != e.data.renderRectangleEvent.prev.y)
            // {
            //     interpolatedRect.y = static_cast<int>(e.data.renderRectangleEvent.now.y * alpha + e.data.renderRectangleEvent.prev.y * (1.0 - alpha));
            // }
            SDL_RenderFillRect(this->renderer, &interpolatedRect);
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