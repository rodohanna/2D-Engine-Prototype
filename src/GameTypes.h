#ifndef GAMETYPES_h_
#define GAMETYPES_h_

#include <SDL2/SDL.h>

typedef SDL_Rect Rect;
typedef SDL_Color Color;

struct V2
{
    double x, y;
};

struct FRect
{
    double x, y;
    int w, h;
};

#endif