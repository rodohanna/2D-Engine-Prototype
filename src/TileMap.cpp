#include "TileMap.h"
#include "Assets.h"

TileMap makeTileMap(Uint32 numTiles)
{
    if (numTiles == 0)
    {
        printf("Warning: attempt to make a tilemap with 0 tiles\n");
    }
    TileMap tileMap;
    tileMap.tiles.resize(numTiles);
    return tileMap;
}

TileMap *loadTileMap(std::string textureKey, Uint32 tileWidth, Uint32 tileHeight)
{
    std::unique_ptr<Texture> texture = getTexture(textureKey);
    if (texture == nullptr)
    {
        printf("TileMap %s could not be loaded\n", "tiles");
        return NULL;
    }
    TileMap *tileMap = new TileMap();
    // TODO:
    // Do some math to figure out if the texture size makes sense with the tile dimensions
    // If too big, print a warning.
    // If too small, fail.
    // Create Tiles with clips
    // Return tilemap
    return tileMap;
}