#ifndef PANEL_h_
#define PANEL_h_

#include <SDL2/SDL.h>
#include "GameEntity.h"
#include "GameEvent.h"
#include "GameState.h"

struct Panel : GameEntity
{
    Panel(SDL_Rect box);
    ~Panel();
    void render(SDL_Renderer *renderer, SDL_Rect &camera);
    bool update(GameState &state);
    void handleEvent(GameEvent *e, GameState *state);
    SDL_Rect mBox;
};

#endif