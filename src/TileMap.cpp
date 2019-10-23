#include "TileMap.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include "Assets.h"
#include "Physics.h"
#include "Input.h"
#include "GameEvent.h"

TileMapTile::TileMapTile(Tile *tile, SDL_Rect box) : mTile(tile), mBox(box){};

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
        printf("Creating tile at (%d, %d) with index %d\n", x, y, tileIndex);
        SDL_Rect box = {x, y, tile->mBox.w * mScale, tile->mBox.h * mScale};
        mTiles.push_back(std::make_unique<TileMapTile>(TileMapTile(tile, box)));
        x += box.w;
        if (x >= mapWidth * box.w)
        {
            x = 0;
            y += box.h;
        }
    }
    if (mTiles.size() > 0)
    {
        Tile *tile = mTiles[0]->mTile;
        mWidth = mapWidth * tile->mBox.w * mScale;
        mHeight = mapHeight * tile->mBox.h * mScale;
    }
    map.close();
    printf("TileMap loaded %lu tiles\n", mTiles.size());
    printf("Map Width: %d Map Height: %d\n", mWidth, mHeight);
}

int TileMap::render(SDL_Renderer *renderer, SDL_Rect &camera, int mouseX, int mouseY)
{
    int numTilesRendered = 0;
    for (int i = 0; i < mTiles.size(); ++i)
    {
        TileMapTile *tile = mTiles[i].get();
        if (checkCollision(camera, tile->mBox))
        {
            ++numTilesRendered;
            int x = tile->mBox.x - camera.x, y = tile->mBox.y - camera.y;
            tile->mTile->render(renderer, camera, x, y, mScale);

            SDL_Rect clip = tile->mTile->mClip;
            int tileWidth = clip.w * mScale, tileHeight = clip.h * mScale;
            if (x < mouseX && x + tileWidth > mouseX && y < mouseY && y + tileHeight > mouseY)
            {
                SDL_Rect rect = {x, y, tileWidth, tileHeight};
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x7F);
                SDL_RenderDrawRect(renderer, &rect);
                std::stringstream ss("");
                int tileX = (x + camera.x) / tileWidth, tileY = (y + camera.y) / tileHeight;
                ss << tileX << ", " << tileY;
                SDL_Color color = {0x11, 0x11, 0x11, 0xFF};
                auto label = Texture::makeTextureFromText(ss.str(), color, getFont("standard_font"), renderer);
                label->render(renderer, x + ((tileWidth - label->mWidth) / 2), y + ((tileHeight - label->mHeight) / 2));
                if (isInputActive(LEFT_MOUSE_JUST_PRESSED))
                {
                    GameEvent e;
                    e.type = TILE_CLICKED;
                    TileClickedEvent tE = {e.type, tileX, tileY, tile};
                    e.tileClickedEvent = tE;
                    registerGameEvent(e);
                    printf("left mouse just pressed, registering e: %d\n", e.type);
                }
            }
        }
    }
    return numTilesRendered;
}