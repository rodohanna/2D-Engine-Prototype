#ifndef TILEMAP_h_
#define TILEMAP_h_

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "TileSet.h"
#include "Tile.h"

struct TileMapTile
{
    TileMapTile(Tile *tile, SDL_Rect box);
    Tile *mTile;
    SDL_Rect mBox;
};

struct TileMap
{
    TileMap(TileSet &tileSet, std::string mapPath, int scale = 1);
    int render(SDL_Renderer *renderer, SDL_Rect &camera, int mouseX, int mouseY);
    std::vector<std::unique_ptr<TileMapTile>> mTiles;
    int mWidth, mHeight, mScale;
};

#endif