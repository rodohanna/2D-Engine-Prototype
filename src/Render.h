#ifndef RENDER_h_
#define RENDER_h_

#include "GameTypes.h"

const static size_t RENDER_QUEUE_SIZE = 4096;

namespace Render
{
enum Layer
{
    GUI_LAYER,
    WORLD_LAYER
};
enum EventType
{
    RENDER_RECTANGLE,
    RENDER_TEXTURE
};
struct RenderRectangleEvent
{
    Rect box;
    Color color;
    bool filled;
};

struct RenderTextureEvent
{
    size_t texture_index;
    Rect clip;
    V2 position;
    size_t scale;
    bool has_clip;
};
struct Event
{
    Render::Layer layer;
    Render::EventType type;
    Rect overflow_clip;
    bool has_overflow_clip;
    size_t z_index;
    union {
        Render::RenderRectangleEvent render_rectangle_event;
        Render::RenderTextureEvent render_texture_event;
    } data;
};
void render_texture(Render::Layer layer, size_t texture_index, V2 &position, Rect *overflow_clip = nullptr, size_t scale = 1, size_t z_index = 1);
void render_texture(Render::Layer layer, size_t texture_index, Rect &clip, V2 &position, Rect *overflow_clip = nullptr, size_t scale = 1, size_t z_index = 1);
void render_rectangle(Render::Layer layer, const Rect &box, const Color &color, bool filled = false, size_t z_index = 1);
void perform_render();
}; // namespace Render

#endif