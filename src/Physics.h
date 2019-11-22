#ifndef PHYSICS_h_
#define PHYSICS_h_

#include "GameTypes.h"

namespace Physics
{
bool check_collision(Rect *a, Rect *b);
bool check_point_in_rect(V2 *point, SDL_Rect *rect);
int lerp(double start, double end, double normal);
}; // namespace Physics

#endif