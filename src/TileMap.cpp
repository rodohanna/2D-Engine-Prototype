#include "TileMap.h"

#include <fstream>
#include "Physics.h"

TileMap::TileMap(TileSet &tileSet, std::string mapPath, int scale)
{
    mScale = scale;
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
            printf("Reading from map %s failed, quiting early\n", mapPath.c_str());
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
        tileMapTile.mTile = tile;
        tileMapTile.mBox.x = x;
        tileMapTile.mBox.y = y;
        tileMapTile.mBox.w = tile->mBox.w * mScale;
        tileMapTile.mBox.h = tile->mBox.h * mScale;
        x += tileMapTile.mBox.w;
        if (x >= mapWidth * tileMapTile.mBox.w)
        {
            x = 0;
            y += tileMapTile.mBox.h;
        }
        mTiles.push_back(tileMapTile);
    }
    if (mTiles.size() > 0)
    {
        Tile *tile = mTiles[0].mTile;
        mWidth = mapWidth * tile->mBox.w * mScale;
        mHeight = mapHeight * tile->mBox.h * mScale;
    }
    map.close();
    printf("TileMap loaded %lu tiles\n", mTiles.size());
    printf("Map Width: %d Map Height: %d\n", mWidth, mHeight);
}

int TileMap::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    int numTilesRendered = 0;
    for (auto tile : mTiles)
    {
        if (checkCollision(camera, tile.mBox))
        {
            ++numTilesRendered;
            tile.mTile->render(renderer, tile.mBox.x - camera.x, tile.mBox.y - camera.y, mScale);
        }
    }
    return numTilesRendered;
}