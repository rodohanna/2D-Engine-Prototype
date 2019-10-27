#ifndef PLOT_h_
#define PLOT_h_
#include <SDL2/SDL.h>
#include "GameEntity.h"
#include "GameState.h"
#include "TileMap.h"

struct Plot : GameEntity
{
    Plot(TileMap *t);
    ~Plot();
    bool update(GameState &state);
    void render(SDL_Renderer *renderer, SDL_Rect &camera);
    TileMap *mTileMap;
    // debug
    SDL_Rect mDebugRect;
};

#endif