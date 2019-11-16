#ifndef GAMETYPES_h_
#define GAMETYPES_h_

#include "SDLWrapper.h"

typedef SDL_Rect Rect;
typedef SDL_Color Color;
typedef TTF_Font Font;

struct V2
{
    int x, y;
};

struct FRect
{
    double x, y;
    int w, h;
};

#endif