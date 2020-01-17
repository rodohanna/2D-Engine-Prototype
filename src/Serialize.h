#ifndef SERIALIZE_h_
#define SERIALIZE_h_

#include "Entity.h"

namespace Serialize
{
struct LoadMapResult
{
    ECS::Manager entity_manager;
    bool success;
};
bool save_game(ECS::Manager *, std::string file);
LoadMapResult load_game(std::string file);
std::vector<ECS::Entity> load_things(std::string directory);
} // namespace Serialize

#endif