#ifndef EVENTS_h_
#define EVENTS_h_

#include "SDLWrapper.h"
#include "GameTypes.h"
#include "Assets.h"
#include <string>
#include <vector>
#include <memory>

// Input Events
enum InputEventType
{
    KEY_DOWN,
    KEY_UP
};

enum KeyEventType
{
    W_KEY,
    A_KEY,
    S_KEY,
    D_KEY,
};

struct KeyEvent
{
    KeyEventType key;
};

struct InputEvent
{
    InputEventType type;
    union {
        KeyEvent keyEvent;
    } data;
};

// Render Events
enum RenderEventType
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

struct RenderEvent
{
    RenderEventType type;
    size_t z_index;
    union {
        RenderRectangleEvent render_rectangle_event;
        RenderTextureEvent render_texture_event;
    } data;
};

namespace Events
{
RenderEvent createRenderTextureEvent(size_t texture_index, V2 &position, size_t scale = 1, size_t z_index = 1);
RenderEvent createRenderTextureEvent(size_t texture_index, Rect &clip, V2 &position, size_t scale = 1, size_t z_index = 1);
RenderEvent createRenderRectangleEvent(const Rect &box, const Color &color, bool filled = false, size_t z_index = 1);
}; // namespace Events

#endif