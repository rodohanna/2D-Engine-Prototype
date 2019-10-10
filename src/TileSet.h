#ifndef TILEMAP_h_
#define TILEMAP_h_
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Tile.h"

struct TileSet
{
    TileSet(std::string textureKey, int tileWidth, int tileHeight, int numTiles = 0);
    void render(SDL_Renderer *renderer, int x = 0, int y = 0);
    std::vector<Tile> mTiles;
    int mWidth, mHeight;
};

#endif