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
            column.push_back({{}, -1});
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
    int texture_index = Assets::get_texture_index("tilesheet-colored");
    for (int i = 0; i < dimensions->x; ++i)
    {
        for (int j = 0; j < dimensions->y; ++j)
        {
            ECS::Tile t;
            t.empty = true;
            ECS::Entity tile_entity;
            ECS::Component render_component;
            render_component.type = ECS::RENDER;
            render_component.strings.push_back("tilesheet-transparent");
            render_component.data.r = {
                {0, 0, 32, 32},
                Render::WORLD_LAYER,
                texture_index,
                static_cast<int>(render_component.strings.size() - 1),
                1,
                1,
                true};
            ECS::Component position_component;
            position_component.type = ECS::POSITION;
            position_component.data.p = {
                i * map.cell_size,
                j * map.cell_size};
            tile_entity.components[render_component.type] = render_component;
            tile_entity.components[position_component.type] = position_component;
            t.tile_entity = tile_entity;
            map.grid[i][j].tile = t;
            map.grid[i][j].has_entity = false;
        }
    }
    entity_manager.map = map;

    return {entity_manager};
}