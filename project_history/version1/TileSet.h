#ifndef TILESET_h_
#define TILESET_h_
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Tile.h"

struct TileSet
{
    TileSet(std::string textureKey, int tileWidth, int tileHeight, int numTiles = 0);
    void render(SDL_Renderer *renderer, SDL_Rect camera, int x = 0, int y = 0);
    std::vector<std::unique_ptr<Tile>> mTiles;
    int mWidth, mHeight;
};

#endif