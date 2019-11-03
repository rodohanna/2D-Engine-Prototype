#ifndef GAMESTATE_h_
#define GAMESTATE_h_

#include <SDL2/SDL.h>
#include "GameTypes.h"

struct GameState
{
    SDL_Rect camera;
    V2 mapDimensions;
    V2 mouseCoords;
};

#endif