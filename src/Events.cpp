#include "Events.h"

RenderEvent Events::createRenderTextureEvent(size_t texture_index, V2 &position, size_t scale, size_t z_index)
{
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.z_index = z_index;
    e.data.render_texture_event = {texture_index, {}, position, scale, false};
    return e;
}

RenderEvent Events::createRenderTextureEvent(size_t texture_index, Rect &clip, V2 &position, size_t scale, size_t z_index)
{
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.z_index = z_index;
    e.data.render_texture_event = {texture_index, clip, position, scale, true};
    return e;
}