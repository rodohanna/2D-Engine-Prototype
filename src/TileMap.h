#ifndef TILEMAP_h_
#define TILEMAP_h_
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Tile.h"

struct TileMap
{
    std::vector<Tile> tiles;
};

TileMap makeTileMap(Uint32 numTiles);
TileMap *loadTileMap(std::string path, Uint32 tileWidth, Uint32 tileHeight);

#endif