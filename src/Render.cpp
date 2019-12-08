#include "Render.h"
#include "Assets.h"
#include "Window.h"
#include "SDLWrapper.h"
#include <stdio.h>
#include <algorithm>

Render::Event render_queue[RENDER_QUEUE_SIZE];
Render::Event world_layer_buffer[RENDER_QUEUE_SIZE];
Render::Event gui_layer_buffer[RENDER_QUEUE_SIZE];
int render_queue_length = 0;

void Render::render_texture(Render::Layer layer, int texture_index, V2 &position, Rect *overflow_clip, int scale, int z_index)
{
    Render::Event e;
    e.layer = layer;
    e.type = Render::EventType::RENDER_TEXTURE;
    if (overflow_clip == nullptr)
    {
        e.has_overflow_clip = false;
    }
    else
    {
        e.has_overflow_clip = true;
        e.overflow_clip = *overflow_clip;
    }
    e.z_index = z_index;
    e.data.render_texture_event = {texture_index, {}, position, scale, false};
    render_queue[render_queue_length++] = e;
}

void Render::render_texture(Render::Layer layer, int texture_index, Rect &clip, V2 &position, Rect *overflow_clip, int scale, int z_index)
{
    Render::Event e;
    e.layer = layer;
    e.type = Render::EventType::RENDER_TEXTURE;
    if (overflow_clip == nullptr)
    {
        e.has_overflow_clip = false;
    }
    else
    {
        e.has_overflow_clip = true;
        e.overflow_clip = *overflow_clip;
    }
    e.z_index = z_index;
    e.has_overflow_clip = false;
    e.data.render_texture_event = {texture_index, clip, position, scale, true};
    render_queue[render_queue_length++] = e;
}

void Render::render_rectangle(Render::Layer layer, const Rect &box, const Color &color, bool filled, int z_index)
{
    Render::Event e;
    e.layer = layer;
    e.type = Render::EventType::RENDER_RECTANGLE;
    e.has_overflow_clip = false;
    e.z_index = z_index;
    e.data.render_rectangle_event = {box, color, filled};
    render_queue[render_queue_length++] = e;
}

bool compare_render_events(Render::Event a, Render::Event b)
{
    return a.z_index < b.z_index;
}

void _perform_render(SDL_Renderer *renderer, const Render::Event *render_events, int length)
{
    auto texture_table = Assets::get_texture_table();
    std::vector<Render::Event> render_vector(render_events, render_events + length);
    std::sort(render_vector.begin(), render_vector.end(), compare_render_events);
    for (Render::Event &e : render_vector)
    {
        switch (e.type)
        {
        case Render::EventType::RENDER_RECTANGLE:
        {
            if (e.has_overflow_clip)
            {
                SDL_RenderSetClipRect(renderer, &e.overflow_clip);
            }
            SDL_SetRenderDrawColor(
                renderer,
                e.data.render_rectangle_event.color.r,
                e.data.render_rectangle_event.color.g,
                e.data.render_rectangle_event.color.b,
                e.data.render_rectangle_event.color.a);
            if (e.data.render_rectangle_event.filled)
            {
                SDL_RenderFillRect(renderer, &e.data.render_rectangle_event.box);
            }
            else
            {
                SDL_RenderDrawRect(renderer, &e.data.render_rectangle_event.box);
            }
            SDL_RenderSetClipRect(renderer, nullptr);
            break;
        }
        case Render::EventType::RENDER_TEXTURE:
        {
            if (e.has_overflow_clip)
            {
                SDL_RenderSetClipRect(renderer, &e.overflow_clip);
            }
            int texture_index = e.data.render_texture_event.texture_index;
            if (texture_index >= 0 && texture_index < texture_table->size())
            {
                Texture *texture = texture_table->at(texture_index).get();
                if (texture == nullptr)
                {
                    printf("Error: RenderSystem received event with texture index to nullptr: %d\n", texture_index);
                    SDL_RenderSetClipRect(renderer, nullptr);
                    break;
                }
                Rect *clip = nullptr;
                if (e.data.render_texture_event.has_clip)
                {
                    clip = &e.data.render_texture_event.clip;
                }
                texture->render(renderer, e.data.render_texture_event.position, clip, e.data.render_texture_event.scale);
            }
            SDL_RenderSetClipRect(renderer, nullptr);
            break;
        }
        }
    }
}

void Render::perform_render()
{
    double world_render_scale = Window::get_world_render_scale();
    double gui_render_scale = Window::get_gui_render_scale();
    int world_buffer_length = 0;
    int gui_buffer_length = 0;
    for (int i = 0; i < render_queue_length; ++i)
    {
        Render::Event e = render_queue[i];
        if (e.layer == Render::Layer::GUI_LAYER)
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
    render_queue_length = 0;
    auto renderer = SDL::get_renderer();
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderSetScale(renderer, world_render_scale, world_render_scale);
    _perform_render(renderer, world_layer_buffer, world_buffer_length);

    SDL_RenderSetScale(renderer, gui_render_scale, gui_render_scale);
    _perform_render(renderer, gui_layer_buffer, gui_buffer_length);

    SDL_RenderPresent(renderer);
}