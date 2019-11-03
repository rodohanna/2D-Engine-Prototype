#ifndef EVENTS_h_
#define EVENTS_h_

#include <SDL2/SDL.h>

enum RenderEventType
{
    RENDER_RECTANGLE
};

struct RenderRectangleEvent
{
    SDL_Rect rect;
    SDL_Color color;
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