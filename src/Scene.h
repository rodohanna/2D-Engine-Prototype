#ifndef SCENE_h_
#define SCENE_h_

#include "Entity.h"
#include "EventBus.h"
#include "World.h"
#include <memory>
#include <vector>
#include <string>

struct Scene
{
    Scene(EventBus *);
    void update(double ts);
    EventBus *event_bus;
    std::unique_ptr<ChunkManager> chunk_manager;
};

#endif