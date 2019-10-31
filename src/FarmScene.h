#ifndef FARMSCENE_h_
#define FARMSCENE_h_

#include <SDL2/SDL.h>
#include <vector>
#include "GameEntity.h"
#include "GameState.h"
#include "TileMap.h"
#include "EventBus.h"

struct FarmScene : Subscriber
{
    FarmScene(TileMap *tileMap);
    ~FarmScene();
    void update(GameState &state);
    void render(SDL_Renderer *renderer, GameState &state);
    void handleEvent(GameEvent *e, GameState *s);
    std::vector<std::unique_ptr<GameEntity>> mGameEntities;
    std::vector<GameEntity *> mRenderList;
    TileMap *mTileMap;
};

#endif