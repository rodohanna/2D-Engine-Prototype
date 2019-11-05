#ifndef EVENTS_h_
#define EVENTS_h_

#include <SDL2/SDL.h>
#include "GameTypes.h"

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

enum RenderEventType
{
    RENDER_RECTANGLE
};

struct RenderRectangleEvent
{
    Rect rect;
    Color color;
};

struct RenderTextureEvent
{
    size_t textureId;
};

struct RenderEvent
{
    RenderEventType type;
    union {
        RenderRectangleEvent renderRectangleEvent;
        RenderTextureEvent renderTextureEvent;
    } data;
};

#endif