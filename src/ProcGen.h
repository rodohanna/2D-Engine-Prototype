#ifndef PROCGEN_h_
#define PROCGEN_h_

#include "GameTypes.h"
#include "Entity.h"
#include "Map.h"
#include <unordered_map>

namespace ProcGen
{
struct Rules
{
    int tree_weight;
    int ground_weight;
};
struct Return
{
    ECS::Manager entity_manager;
    Map map;
};
Return generate_map(Rules *rules, V2 *dimensions);
}; // namespace ProcGen

#endif