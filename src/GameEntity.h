#ifndef GAMEENTITY_h_
#define GAMEENTITY_h_
#include <SDL2/SDL.h>
#include "GameState.h"

struct GameEntity
{
    virtual bool update(GameState &state) = 0;
    virtual void render(SDL_Renderer *renderer, SDL_Rect &camera) = 0;
};

#endif