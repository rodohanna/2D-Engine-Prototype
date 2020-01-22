#ifndef RENDER_h_
#define RENDER_h_

#include "GameTypes.h"

const static int RENDER_QUEUE_SIZE = 16384;

namespace Render
{
namespace Z_Index
{
enum World
{
    TILE_BASE_LAYER,
    FLOOR_LAYER,
    ENTITY_LAYER
};
};
enum Layer
{
    GUI_LAYER,
    WORLD_LAYER
};
enum EventType
{
    RENDER_RECTANGLE,
    RENDER_TEXTURE,
    RENDER_LINE
};
struct RenderRectangleEvent
{
    Rect box;
    Color color;
    bool filled;
};

struct RenderLineEvent
{
    V2 start;
    V2 end;
    Color color;
};

struct RenderTextureEvent
{
    Rect clip;
    V2 position;
    int texture_index;
    int scale;
    bool has_clip;
};
struct Event
{
    Render::Layer layer;
    Render::EventType type;
    Rect overflow_clip;
    bool has_overflow_clip;
    int z_index;
    union {
        Render::RenderRectangleEvent render_rectangle_event;
        Render::RenderTextureEvent render_texture_event;
        Render::RenderLineEvent render_line_event;
    } data;
};
void render_texture(
    Render::Layer layer,
    int texture_index,
    V2 &position,
    Rect *overflow_clip = nullptr,
    int scale = 1,
    int z_index = 1);
void render_texture(
    Render::Layer layer,
    int texture_index,
    Rect &clip,
    V2 &position,
    Rect *overflow_clip = nullptr,
    int scale = 1,
    int z_index = 1);
void render_rectangle(
    Render::Layer layer,
    const Rect &box,
    const Color &color,
    bool filled = false,
    int z_index = 1);
void render_line(
    Render::Layer layer,
    V2 *start,
    V2 *end,
    Color *color,
    int z_index = 1);
void perform_render();
}; // namespace Render

#endif