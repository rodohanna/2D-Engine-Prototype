#ifndef TILE_h_
#define TILE_h_
#include <SDL2/SDL.h>
#include "Texture.h"

struct Tile
{
    Tile(SDL_Rect clip, SDL_Rect box, Texture *texture);
    void render(SDL_Renderer *renderer, SDL_Rect camera, int x = 0, int y = 0, int scale = 1);
    SDL_Rect mClip;
    SDL_Rect mBox;
    Texture *mTexture;
};

#endif