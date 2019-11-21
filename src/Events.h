#ifndef EVENTS_h_
#define EVENTS_h_

#include "SDLWrapper.h"
#include "GameTypes.h"
#include "Assets.h"

// Input Events
enum InputEventType
{
    KEY_DOWN,
    KEY_UP,
    WINDOW_RESIZE,
    TEXT_INPUT,
    MOUSE_CLICK,
    GUI_FOCUSED,
    GUI_UNFOCUSED
};

enum KeyEventType
{
    W_KEY,
    A_KEY,
    S_KEY,
    D_KEY,
    BACKSPACE_KEY,
    ENTER_KEY
};

struct KeyEvent
{
    KeyEventType key;
};

struct WindowResizeEvent
{
    V2 new_size;
};

struct TextInputEvent
{
    char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
    bool is_backspace;
};

enum MouseButton
{
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT
};

struct MouseClickEvent
{
    MouseButton button;
};

struct InputEvent
{
    InputEventType type;
    union {
        KeyEvent key_event;
        WindowResizeEvent resize_event;
        TextInputEvent text_input_event;
        MouseClickEvent mouse_click_event;
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
    Rect overflow_clip;
    bool has_overflow_clip;
    size_t z_index;
    union {
        RenderRectangleEvent render_rectangle_event;
        RenderTextureEvent render_texture_event;
    } data;
};

namespace Events
{
RenderEvent createRenderTextureEvent(size_t texture_index, V2 &position, Rect *overflow_clip = nullptr, size_t scale = 1, size_t z_index = 1);
RenderEvent createRenderTextureEvent(size_t texture_index, Rect &clip, V2 &position, Rect *overflow_clip = nullptr, size_t scale = 1, size_t z_index = 1);
RenderEvent createRenderRectangleEvent(const Rect &box, const Color &color, bool filled = false, size_t z_index = 1);
}; // namespace Events

#endif