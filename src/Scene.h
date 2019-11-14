#ifndef SCENE_h_
#define SCENE_h_

#include "Entity.h"
#include "EventBus.h"
#include <memory>
#include <vector>
#include <string>

struct Scene
{
    Scene(EventBus *);
    void load_from_file(std::string path);
    void update(double ts);
    EventBus *event_bus;
    std::vector<std::unique_ptr<IEntity>> map;
};

#endif