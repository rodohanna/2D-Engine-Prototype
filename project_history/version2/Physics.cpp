#include "Physics.h"

bool Physics::check_collision(Rect *a, Rect *b)
{
    int leftA = a->x;
    int rightA = a->x + a->w;
    int topA = a->y;
    int bottomA = a->y + a->h;

    int leftB = b->x;
    int rightB = b->x + b->w;
    int topB = b->y;
    int bottomB = b->y + b->h;

    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    return true;
}

bool Physics::check_point_in_rect(V2 *point, SDL_Rect *rect)
{
    int x = point->x;
    int y = point->y;
    return rect->x < x && rect->x + rect->w > x && rect->y < y && rect->y + rect->h > y;
}

int Physics::lerp(double start, double end, double normal)
{
    return (1 - normal) * start + normal * end;
}