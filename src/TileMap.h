#ifndef TILEMAP_h_
#define TILEMAP_h_

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "TileSet.h"
#include "Tile.h"
#include "GameState.h"
#include "Physics.h"

struct TileMapTile
{
    TileMapTile(Tile *tile, SDL_Rect box);
    Tile *mTile;
    SDL_Rect mBox;
};

struct TileMap
{
    TileMap(TileSet &tileSet, std::string mapPath, int scale = 1);
    int render(SDL_Renderer *renderer, GameState &state);
    TileMapTile *getTileFromPosition(GameState &state, V2 &position);
    std::vector<std::unique_ptr<TileMapTile>> mTiles;
    int mWidth, mHeight, mScale;
    TileSet *mTileSet;
};

#endif