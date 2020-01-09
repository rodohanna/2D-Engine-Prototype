#ifndef SERIALIZE_h_
#define SERIALIZE_h_

#include "Entity.h"

namespace Serialize
{
struct LoadMapResult
{
    ECS::Map map;
    bool success;
};
bool save_map(ECS::Map *, std::string file);
LoadMapResult load_map(std::string file);
} // namespace Serialize

#endif