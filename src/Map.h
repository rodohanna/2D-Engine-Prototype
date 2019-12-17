#ifndef MAP_h_
#define MAP_h_

#include "GameTypes.h"
#include <vector>

struct Cell
{
    int entity_id;
};

struct Map
{
    std::vector<std::vector<Cell>> grid;
    V2 dimensions;
    V2 pixel_dimensions;
    int cell_size;
};

#endif