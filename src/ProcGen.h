#ifndef PROCGEN_h_
#define PROCGEN_h_

#include "Map.h"
#include "GameTypes.h"
#include <unordered_map>

namespace ProcGen
{
struct Rules
{
    int tree_weight;
    int ground_weight;
};
Map generate_map(Rules *rules, V2 *dimensions);
}; // namespace ProcGen

#endif