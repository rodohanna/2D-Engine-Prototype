#ifndef MAP_h_
#define MAP_h_

#include "Entity.h"
#include <vector>

struct Cell
{
    int entity_id;
};

struct Map
{
    ECS::Manager entity_manager;
    std::vector<std::vector<Cell>> grid;
};

#endif