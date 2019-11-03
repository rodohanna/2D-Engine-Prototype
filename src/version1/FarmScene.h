#ifndef FARMSCENE_h_
#define FARMSCENE_h_

#include <SDL2/SDL.h>
#include <vector>
#include "GameEntity.h"
#include "GameState.h"
#include "TileMap.h"
#include "EventBus.h"
#include "Player.h"
#include "FarmTile.h"
#include "Label.h"

struct FarmScene : Subscriber
{
    FarmScene(Player player, TileMap *tileMap);
    ~FarmScene();
    void update(GameState &state);
    void render(SDL_Renderer *renderer, GameState &state);
    void handleEvent(GameEvent *e, GameState *s);

    Player mPlayer;
    Label mLabel;
    std::vector<FarmTile> mFarmTiles;
    std::vector<GameEntity *> mRenderList;
    TileMap *mTileMap;
};

#endif