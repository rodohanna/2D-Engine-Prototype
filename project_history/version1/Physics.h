#ifndef PHYSICS_h_
#define PHYSICS_h_
#include <SDL2/SDL.h>
#include "GameTypes.h"

bool checkCollision(SDL_Rect &a, SDL_Rect &b);
bool checkPointInRect(V2 &point, SDL_Rect &rect);

#endif