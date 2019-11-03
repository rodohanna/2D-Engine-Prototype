#include "TileSet.h"
#include "Assets.h"

TileSet::TileSet(std::string textureKey, int tileWidth, int tileHeight, int numTiles)
{
    if (numTiles > 0)
    {
        mTiles.reserve(numTiles);
    }
    Texture *texture = getTexture(textureKey);
    if (texture == NULL)
    {
        printf("TileMap %s could not be loaded\n", "tiles");
        return;
    }
    else if (tileWidth == 0 || tileHeight == 0)
    {
        printf("Error: Invalid tile dimensions supplied to TileMap (%d, %d)\n", tileWidth, tileHeight);
        return;
    }
    else if (texture->mWidth < tileWidth || texture->mHeight < tileHeight)
    {
        printf("Error: Tile dimensions supplied to TileMap are bigger than supplied texture\n");
    }

    if (texture->mWidth % tileWidth != 0)
    {
        printf("Warning: TileMap %s loaded with width %d does not clip evenly.\n", textureKey.c_str(), tileWidth);
    }
    if (texture->mHeight % tileHeight != 0)
    {
        printf("Warning: TileMap %s loaded with height %d does not clip evenly.\n", textureKey.c_str(), tileHeight);
    }
    int numberOfTileColumns = texture->mWidth / tileWidth;
    int numberOfTileRows = texture->mHeight / tileHeight;
    mWidth = numberOfTileColumns * tileWidth;
    mHeight = numberOfTileRows * tileHeight;
    for (int i = 0; i < numberOfTileColumns; i++)
    {
        for (int j = 0; j < numberOfTileRows; j++)
        {
            SDL_Rect box = {(i * tileWidth), (j * tileHeight), tileWidth, tileHeight};
            printf("Tile index %lu clip %d %d %d %d\n", mTiles.size(), box.x, box.y, box.w, box.h);
            std::unique_ptr<Tile> tile = std::make_unique<Tile>(Tile(box, box, texture));
            mTiles.push_back(std::move(tile));
        }
    }
    printf("Tiles for %s loaded!\n", textureKey.c_str());
}

void TileSet::render(SDL_Renderer *renderer, SDL_Rect camera, int x, int y)
{
    for (auto &tile : mTiles)
    {
        tile->render(renderer, camera, x + tile->mBox.x, y + tile->mBox.y);
    }
}