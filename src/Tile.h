#ifndef TILE_h_
#define TILE_h_
#include <SDL2/SDL.h>
#include "Texture.h"

struct Tile
{
    SDL_Rect clip;
    SDL_Rect box;
    Texture *texture;
};

#endif