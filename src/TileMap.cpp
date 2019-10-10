#include "TileMap.h"

#include <fstream>

TileMap::TileMap(TileSet &tileSet, std::string mapPath)
{
    std::ifstream map(mapPath);
    if (map.fail())
    {
        map.close();
        printf("Error: Unable to load map file at %s\n", mapPath.c_str());
        return;
    }
    int tileIndex = -1;
    int x = 0, y = 0;
    int mapWidth = 0, mapHeight = 0;
    map >> mapWidth >> mapHeight;
    printf("width height %d %d\n", mapWidth, mapHeight);
    while (map >> tileIndex)
    {
        if (map.fail())
        {
            printf("breaking early...\n");
            break;
        }
        if (tileIndex < 0 || tileIndex > tileSet.mTiles.size() - 1)
        {
            printf("Error: Recieved invalid tile index %d for map %s\n", tileIndex, mapPath.c_str());
            map.close();
            return;
        }
        Tile *tile = tileSet.mTiles[tileIndex].get();
        TileMapTile tileMapTile;
        printf("Creating tile at (%d, %d) with index %d\n", x, y, tileIndex);
        tileMapTile.tile = tile;
        tileMapTile.x = x;
        tileMapTile.y = y;
        x += tile->mBox.w;
        if (x >= mapWidth * tile->mBox.w)
        {
            x = 0;
            y += tile->mBox.h;
        }
        mTiles.push_back(tileMapTile);
    }
    if (mTiles.size() > 0)
    {
        Tile *tile = mTiles[0].tile;
        mWidth = mapWidth * tile->mBox.w;
        mHeight = mapHeight * tile->mBox.h;
    }
    map.close();
    printf("TileMap loaded %lu tiles\n", mTiles.size());
    printf("Map Width: %d Map Height: %d\n", mWidth, mHeight);
}

void TileMap::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    for (auto tile : mTiles)
    {
        tile.tile->render(renderer, tile.x - camera.x, tile.y - camera.y);
    }
}