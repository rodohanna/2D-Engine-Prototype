#ifndef SCENERY_h_
#define SCENERY_h_

#include "Entity.h"
#include "GameTypes.h"
#include "EventBus.h"

struct BackGround : IEntity
{
    BackGround(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index);
    void update(double delta);
    Rect clip;
    V2 position;
    EventBus *event_bus;
    size_t texture_index;
};

struct Tree : IEntity
{
    Tree(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index);
    void update(double delta);
    Rect clip;
    V2 position;
    EventBus *event_bus;
    size_t texture_index;
};

#endif