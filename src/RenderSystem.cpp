#include "RenderSystem.h"
#include "Assets.h"
#include <stdio.h>

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : renderer(r), event_bus(eB)
{
    this->event_bus->subscribe_to_render_Events(this);
    this->texture_table = Assets::get_texture_table();
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
        case RenderEventType::RENDER_RECTANGLE:
        {
            SDL_SetRenderDrawColor(
                renderer,
                e.data.render_rectangle_event.color.r,
                e.data.render_rectangle_event.color.g,
                e.data.render_rectangle_event.color.b,
                e.data.render_rectangle_event.color.a);

            SDL_RenderFillRect(this->renderer, &e.data.render_rectangle_event.box);
            break;
        }
        case RenderEventType::RENDER_TEXTURE:
        {
            size_t texture_index = e.data.render_texture_event.texture_index;
            if (texture_index >= 0 && texture_index < texture_table->size())
            {
                Texture *texture = this->texture_table->at(texture_index).get();
                if (texture == nullptr)
                {
                    printf("Error: RenderSystem received event with texture index to nullptr: %d\n", texture_index);
                    break;
                }
                texture->render(this->renderer, e.data.render_texture_event.position);
            }
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