#ifndef PHYSICS_h_
#define PHYSICS_h_

#include "GameTypes.h"

namespace Physics
{
bool checkCollision(Rect *a, Rect *b);
bool checkPointInRect(V2 *point, SDL_Rect *rect);
}; // namespace Physics

#endif