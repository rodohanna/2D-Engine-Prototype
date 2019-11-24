#ifndef MAPGEN_h_
#define MAPGEN_h_

#include "Entity.h"
#include "GameTypes.h"
#include "EventBus.h"
#include <vector>
#include <memory>
#include <string>

struct Tile
{
    Rect clip;
    size_t texture_index;
    size_t scale;
};

struct ProcGenRules
{
    size_t ground_weight;
    size_t tree_weight;
};

struct Palette
{
    // background
    Tile background_tile;
    // ground
    std::vector<Tile> ground_tiles;
    // tree
    std::vector<Tile> tree_tiles;
    size_t scale;
};

struct Map
{
    std::shared_ptr<IEntity> background;
    std::vector<std::vector<std::shared_ptr<IEntity>>> grid;
};

namespace MapGen
{
Palette load_palette(std::string path);
Map generate_map(Palette *p, ProcGenRules *r, EventBus *e, const V2 &dimensions, const V2 &world_offset);
}; // namespace MapGen

#endif