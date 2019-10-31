#include "FarmScene.h"

// just for proof of concept
#include "Player.h"
#include "Assets.h"
#include "EventBus.h"

FarmScene::FarmScene(TileMap *tileMap) : mTileMap(tileMap)
{
    Player player = Player(getTexture("player"));
    mGameEntities.push_back(std::make_unique<Player>(player));
    subscribe(GameEventType::TILL_SOIL, this);
};

FarmScene::~FarmScene()
{
    unsubscribe(GameEventType::TILL_SOIL, this);
};

void FarmScene::handleEvent(GameEvent *e, GameState *s)
{
    switch (e->eventType)
    {
    case GameEventType::TILL_SOIL:
    {
        TillSoilEvent *tillSoilEvent = static_cast<TillSoilEvent *>(e);
        if (tillSoilEvent != NULL)
        {
            printf("farm: %d %d\n", tillSoilEvent->coords.x, tillSoilEvent->coords.y);
            TileMapTile *tile = mTileMap->getTileFromPosition(*s, tillSoilEvent->coords);
            if (tile != NULL)
            {
                tile->mTile = mTileMap->mTileSet->mTiles[0].get();
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    printf("FarmScene handling event\n");
};

void FarmScene::update(GameState &state)
{
    for (int i = 0; i < mGameEntities.size(); i++)
    {
        auto entity = mGameEntities[i].get();
        entity->update(state);
    }
};

void FarmScene::render(SDL_Renderer *renderer, GameState &state)
{
    mTileMap->render(renderer, state);
    for (int i = 0; i < mGameEntities.size(); i++)
    {
        auto entity = mGameEntities[i].get();
        entity->render(renderer, state.camera);
    }
};