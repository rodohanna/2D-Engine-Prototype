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

namespace MapGen
{
Palette load_palette(std::string path);
std::vector<std::unique_ptr<IEntity>> generate_map(Palette *p, EventBus *e, const V2 &dimensions);
}; // namespace MapGen

#endif