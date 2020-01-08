#include "ProcGen.h"
#include "Assets.h"
#include <stdlib.h>

ProcGen::Return ProcGen::generate_map(ProcGen::Rules *rules, V2 *dimensions)
{
    ECS::Manager entity_manager = ECS::Manager();
    ECS::Map map;
    // Initialize map.
    for (int i = 0; i < dimensions->x; ++i)
    {
        std::vector<ECS::Cell> column;
        for (int j = 0; j < dimensions->y; ++j)
        {
            column.push_back({-1});
        }
        map.grid.push_back(column);
    }
    ECS::Entity player;
    ECS::Component position;
    position.type = ECS::Type::POSITION;
    position.data.p.position = {0, 0};
    player.components[position.type] = position;
    player.components[ECS::Type::CAMERA] = {};
    player.components[ECS::Type::PLAYER_INPUT] = {};
    entity_manager.entities.push_back(player);

    map.dimensions = *dimensions;
    map.cell_size = 32;
    map.pixel_dimensions = {map.dimensions.x * map.cell_size, map.dimensions.y * map.cell_size};
    for (int i = 0; i < dimensions->x; ++i)
    {
        for (int j = 0; j < dimensions->y; ++j)
        {
            ECS::Tile t;
            t.empty = true;
            t.position = {i * map.cell_size, j * map.cell_size};
            map.grid[i][j].tile = t;
        }
    }
    entity_manager.map = map;

    return {entity_manager};
}