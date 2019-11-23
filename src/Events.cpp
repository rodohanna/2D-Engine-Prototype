#include "Events.h"

RenderEvent Events::create_render_texture_event(RenderLayer layer, size_t texture_index, V2 &position, Rect *overflow_clip, size_t scale, size_t z_index)
{
    RenderEvent e;
    e.layer = layer;
    e.type = RenderEventType::RENDER_TEXTURE;
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
    return e;
}

RenderEvent Events::create_render_texture_event(RenderLayer layer, size_t texture_index, Rect &clip, V2 &position, Rect *overflow_clip, size_t scale, size_t z_index)
{
    RenderEvent e;
    e.layer = layer;
    e.type = RenderEventType::RENDER_TEXTURE;
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
    return e;
}

RenderEvent Events::create_render_rectangle_event(RenderLayer layer, const Rect &box, const Color &color, bool filled, size_t z_index)
{
    RenderEvent e;
    e.layer = layer;
    e.type = RenderEventType::RENDER_RECTANGLE;
    e.has_overflow_clip = false;
    e.z_index = z_index;
    e.data.render_rectangle_event = {box, color, filled};
    return e;
}