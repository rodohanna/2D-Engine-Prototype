#ifndef SERIALIZE_h_
#define SERIALIZE_h_

#include "Entity.h"
#include "Build.h"
#include <unordered_map>

namespace Serialize
{
struct LoadThingsResult
{
    std::vector<Build::Buildable> buildables;
};
struct LoadMapResult
{
    ECS::Manager entity_manager;
    bool success;
};
bool save_game(ECS::Manager *, std::string file);
LoadMapResult load_game(std::string file);
LoadThingsResult load_things(std::string directory);
} // namespace Serialize

#endif