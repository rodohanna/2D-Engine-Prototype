#include "Physics.h"

bool Physics::check_collision(Rect *a, Rect *b)
{
    int left_a = a->x;
    int right_a = a->x + a->w;
    int top_a = a->y;
    int bottom_a = a->y + a->h;

    int left_b = b->x;
    int right_b = b->x + b->w;
    int top_b = b->y;
    int bottom_b = b->y + b->h;

    if (bottom_a <= top_b)
    {
        return false;
    }

    if (top_a >= bottom_b)
    {
        return false;
    }

    if (right_a <= left_b)
    {
        return false;
    }

    if (left_a >= right_b)
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