#ifndef GAMESTATE_h_
#define GAMESTATE_h_

#include <SDL2/SDL.h>

struct GameState
{
    SDL_Rect camera;
    int mapWidth, mapHeight;
    int mouseX, mouseY;
};

#endif