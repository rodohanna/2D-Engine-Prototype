#include "RenderSystem.h"
#include "Assets.h"
#include "Window.h"
#include <stdio.h>
#include <algorithm>

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : renderer(r), event_bus(eB)
{
    SDL_RenderSetScale(r, 2.0, 2.0);
    V2 *mouse_position = Window::get_mouse_position();
    SDL_GetMouseState(&mouse_position->x, &mouse_position->y);
    this->event_bus->subscribe_to_render_events(this);
    this->event_bus->subscribe_to_input_events(this);
    this->texture_table = Assets::get_texture_table();
}
RenderSystem::~RenderSystem()
{
    this->event_bus->subscribe_to_render_events(this);
    this->event_bus->unsubscribe_to_input_events(this);
}

bool compare_render_events(RenderEvent a, RenderEvent b)
{
    return a.z_index < b.z_index;
}
void RenderSystem::handle_render_events(const RenderEvent *render_events, size_t length)
{
    std::vector<RenderEvent> render_vector(render_events, render_events + length);
    std::sort(render_vector.begin(), render_vector.end(), compare_render_events);
    SDL_SetRenderDrawColor(this->renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(this->renderer);
    for (RenderEvent &e : render_vector)
    {
        switch (e.type)
        {
        case RenderEventType::RENDER_RECTANGLE:
        {
            if (e.has_overflow_clip)
            {
                SDL_RenderSetClipRect(this->renderer, &e.overflow_clip);
            }
            SDL_SetRenderDrawColor(
                renderer,
                e.data.render_rectangle_event.color.r,
                e.data.render_rectangle_event.color.g,
                e.data.render_rectangle_event.color.b,
                e.data.render_rectangle_event.color.a);
            if (e.data.render_rectangle_event.filled)
            {
                SDL_RenderFillRect(this->renderer, &e.data.render_rectangle_event.box);
            }
            else
            {
                SDL_RenderDrawRect(this->renderer, &e.data.render_rectangle_event.box);
            }
            SDL_RenderSetClipRect(this->renderer, nullptr);
            break;
        }
        case RenderEventType::RENDER_TEXTURE:
        {
            if (e.has_overflow_clip)
            {
                SDL_RenderSetClipRect(this->renderer, &e.overflow_clip);
            }
            size_t texture_index = e.data.render_texture_event.texture_index;
            if (texture_index >= 0 && texture_index < texture_table->size())
            {
                Texture *texture = this->texture_table->at(texture_index).get();
                if (texture == nullptr)
                {
                    printf("Error: RenderSystem received event with texture index to nullptr: %d\n", texture_index);
                    SDL_RenderSetClipRect(this->renderer, nullptr);
                    break;
                }
                Rect *clip = nullptr;
                if (e.data.render_texture_event.has_clip)
                {
                    clip = &e.data.render_texture_event.clip;
                }
                texture->render(this->renderer, e.data.render_texture_event.position, clip, e.data.render_texture_event.scale);
            }
            SDL_RenderSetClipRect(this->renderer, nullptr);
            break;
        }
        }
    }
    SDL_RenderPresent(renderer);
    V2 *mouse_position = Window::get_mouse_position();
    SDL_GetMouseState(&mouse_position->x, &mouse_position->y);
    mouse_position->x /= 2;
    mouse_position->y /= 2;
}
void RenderSystem::handle_input_events(const InputEvent *input_events, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        InputEvent e = input_events[i];
        if (e.type == InputEventType::WINDOW_RESIZE)
        {
            Rect *camera = Window::get_camera();
            camera->w = e.data.resize_event.new_size.x / 2;
            camera->h = e.data.resize_event.new_size.y / 2;
        }
    }
}