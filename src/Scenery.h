#ifndef SCENERY_h_
#define SCENERY_h_

#include "Entity.h"
#include "GameTypes.h"
#include "EventBus.h"

struct Scenery
{
    Scenery(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale = 1);
    Rect clip;
    V2 position;
    EventBus *event_bus;
    size_t texture_index;
    size_t scale;
};

struct BackGround : IEntity, Scenery
{
    BackGround(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale = 1);
    void update(double delta);
};

struct Dirt : IEntity, Scenery
{
    Dirt(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale = 1);
    void update(double delta);
};

struct Tree : IEntity, Scenery
{
    Tree(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale = 1);
    void update(double delta);
};

#endif