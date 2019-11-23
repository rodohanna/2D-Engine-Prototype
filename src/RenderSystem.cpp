#include "RenderSystem.h"
#include "Assets.h"
#include "Window.h"
#include <stdio.h>
#include <algorithm>

Events::RenderEvent world_layer_buffer[RENDER_QUEUE_SIZE];
Events::RenderEvent gui_layer_buffer[RENDER_QUEUE_SIZE];

RenderSystem::RenderSystem(SDL_Renderer *r, EventBus *eB) : gui_render_scale(1.0), world_render_scale(2.0), renderer(r), event_bus(eB)
{
    Rect *camera = Window::get_camera();
    this->update_cameras(camera->w, camera->h);
    this->update_mouse_positions();
    this->event_bus->subscribe_to_render_events(this);
    this->event_bus->subscribe_to_input_events(this);
    this->texture_table = Assets::get_texture_table();
}
RenderSystem::~RenderSystem()
{
    this->event_bus->subscribe_to_render_events(this);
    this->event_bus->unsubscribe_to_input_events(this);
}

bool compare_render_events(Events::RenderEvent a, Events::RenderEvent b)
{
    return a.z_index < b.z_index;
}

void RenderSystem::render(const Events::RenderEvent *render_events, size_t length)
{
    std::vector<Events::RenderEvent> render_vector(render_events, render_events + length);
    std::sort(render_vector.begin(), render_vector.end(), compare_render_events);
    for (Events::RenderEvent &e : render_vector)
    {
        switch (e.type)
        {
        case Events::RenderEventType::RENDER_RECTANGLE:
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
        case Events::RenderEventType::RENDER_TEXTURE:
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
}

void RenderSystem::handle_render_events(const Events::RenderEvent *render_events, size_t length)
{
    size_t world_buffer_length = 0;
    size_t gui_buffer_length = 0;
    for (size_t i = 0; i < length; ++i)
    {
        Events::RenderEvent e = render_events[i];
        if (e.layer == Events::RenderLayer::GUI_LAYER)
        {
            gui_layer_buffer[gui_buffer_length] = e;
            ++gui_buffer_length;
        }
        else
        {
            world_layer_buffer[world_buffer_length] = e;
            ++world_buffer_length;
        }
    }
    SDL_SetRenderDrawColor(this->renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(this->renderer);

    SDL_RenderSetScale(this->renderer, this->world_render_scale, this->world_render_scale);
    this->render(world_layer_buffer, world_buffer_length);

    SDL_RenderSetScale(this->renderer, this->gui_render_scale, this->gui_render_scale);
    this->render(gui_layer_buffer, gui_buffer_length);

    SDL_RenderPresent(renderer);

    this->update_mouse_positions();
}
void RenderSystem::handle_input_events(const Events::InputEvent *input_events, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        Events::InputEvent e = input_events[i];
        if (e.type == Events::InputEventType::WINDOW_RESIZE)
        {
            this->update_cameras(
                e.data.resize_event.new_size.x,
                e.data.resize_event.new_size.y);
        }
    }
}

void RenderSystem::update_cameras(double w, double h)
{
    Rect *camera = Window::get_camera();
    Rect *gui_camera = Window::get_gui_camera();
    camera->w = w / this->world_render_scale;
    camera->h = h / this->world_render_scale;
    gui_camera->w = w / this->gui_render_scale;
    gui_camera->h = h / this->gui_render_scale;
}

void RenderSystem::update_mouse_positions()
{
    V2 *mouse_position = Window::get_mouse_position();
    V2 *gui_mouse_position = Window::get_gui_mouse_position();
    SDL_GetMouseState(&mouse_position->x, &mouse_position->y);
    SDL_GetMouseState(&gui_mouse_position->x, &gui_mouse_position->y);
    mouse_position->x /= this->world_render_scale;
    mouse_position->y /= this->world_render_scale;
    gui_mouse_position->x /= this->gui_render_scale;
    gui_mouse_position->y /= this->gui_render_scale;
}