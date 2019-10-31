#include "FarmScene.h"
#include "GameTypes.h"
#include <sstream>

std::stringstream ss("");

FarmScene::FarmScene(Player player, TileMap *tileMap) : mPlayer(player), mTileMap(tileMap)
{
    subscribe(GameEventType::TILL_SOIL, this);
};

FarmScene::~FarmScene()
{
    unsubscribe(GameEventType::TILL_SOIL, this);
};

void FarmScene::handleEvent(GameEvent *e, GameState *s)
{
    printf("FarmScene::handleEvent handling event %d\n", e->eventType);
    switch (e->eventType)
    {
    case GameEventType::TILL_SOIL:
    {
        printf("FarmScene::handleEvent (TILL_SOIL)\n");
        TillSoilEvent *tillSoilEvent = static_cast<TillSoilEvent *>(e);
        if (tillSoilEvent != NULL)
        {
            printf("FarmScene::handleEvent at coords (%d, %d)\n", tillSoilEvent->coords.x, tillSoilEvent->coords.y);
            TileMapTile *tile = mTileMap->getTileFromPosition(*s, tillSoilEvent->coords);
            if (tile != NULL)
            {
                V2 position = {tile->mBox.x, tile->mBox.y};
                bool exists = false;
                for (FarmTile &tile : mFarmTiles)
                {
                    if (tile.mBox.x == position.x && tile.mBox.y == position.y)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    printf("Adding a new farm tile at (%d, %d)\n", position.x, position.y);
                    mFarmTiles.push_back(FarmTile(position));
                }
                else
                {
                    printf("Tile already exists at (%d, %d)\n", position.x, position.y);
                }
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
};

void FarmScene::update(GameState &state)
{
    mPlayer.update(state);
};

void FarmScene::render(SDL_Renderer *renderer, GameState &state)
{
    mTileMap->render(renderer, state);
    for (FarmTile &tile : mFarmTiles)
    {
        tile.render(renderer, state.camera);
    }
    mPlayer.render(renderer, state.camera);

    // debug stuff
    TileMapTile *tile = mTileMap->getTileFromPosition(state, state.mouseCoords);
    if (tile != NULL)
    {
        SDL_Rect debugRect = {tile->mBox.x - state.camera.x, tile->mBox.y - state.camera.y, tile->mBox.w, tile->mBox.h};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x7F);
        SDL_RenderDrawRect(renderer, &debugRect);
        ss.str("");
        ss << (debugRect.x + state.camera.x) / debugRect.w << "," << (debugRect.y + state.camera.y) / debugRect.h;
        SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
        std::string text = ss.str();
        mLabel.setText(renderer, color, text);
        mLabel.render(renderer, debugRect);
    }
};