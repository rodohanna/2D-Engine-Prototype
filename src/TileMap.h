#ifndef TILEMAP_h_
#define TILEMAP_h_

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "TileSet.h"
#include "Tile.h"

struct TileMapTile
{
    Tile *tile;
    int x, y;
};

struct TileMap
{
    TileMap(TileSet &tileSet, std::string mapPath);
    int render(SDL_Renderer *renderer, SDL_Rect &camera);
    std::vector<TileMapTile> mTiles;
    int mWidth, mHeight;
};

#endif