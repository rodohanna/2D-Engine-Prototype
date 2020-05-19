#include "ProcGen.h"
#include "Assets.h"
#include <stdlib.h>

ProcGen::Return ProcGen::generate_map(ProcGen::Rules *rules, V2 *dimensions)
{
    ECS::Manager entity_manager = ECS::Manager();
    ECS::Map map;
    // Initialize map.
    map.grid = new ECS::Cell *[dimensions->x];
    for (int i = 0; i < dimensions->x; ++i)
    {
        map.grid[i] = new ECS::Cell[dimensions->y];
    }
    for (int i = 0; i < dimensions->x; ++i)
    {
        for (int j = 0; j < dimensions->y; ++j)
        {
            map.grid[i][j].has_entity = false;
            map.grid[i][j].tile.empty = true;
        }
    }
    ECS::Entity player;
    ECS::Component position;
    position.type = ECS::Type::POSITION;
    position.data.p.position = {0, 0};
    ECS::Component camera;
    camera.type = ECS::CAMERA;
    ECS::Component player_input;
    player_input.type = ECS::PLAYER_INPUT;
    player.add_component(&position);
    player.add_component(&camera);
    player.add_component(&player_input);
    entity_manager.entities.push_back(player);

    map.dimensions = *dimensions;
    map.cell_size = 32;
    map.pixel_dimensions = {map.dimensions.x * map.cell_size, map.dimensions.y * map.cell_size};
    int texture_index = Assets::get_texture_index("tilesheet-demo");
    for (int i = 0; i < dimensions->x; ++i)
    {
        for (int j = 0; j < dimensions->y; ++j)
        {
            ECS::Tile t;
            t.empty = true;
            ECS::Entity tile_entity;
            ECS::Component render_component;
            render_component.type = ECS::RENDER;
            render_component.strings.push_back("tilesheet-demo");
            render_component.data.r = {
                {0, 0, 16, 16},
                Render::WORLD_LAYER,
                texture_index,
                static_cast<int>(render_component.strings.size() - 1),
                2,
                Render::Z_Index::TILE_BASE_LAYER,
                true};
            ECS::Component position_component;
            position_component.type = ECS::POSITION;
            position_component.data.p = {
                i * map.cell_size,
                j * map.cell_size};
            tile_entity.add_component(&render_component);
            tile_entity.add_component(&position_component);
            t.tile_entity = tile_entity;
            map.grid[i][j].tile = t;
            map.grid[i][j].has_entity = false;
        }
    }
    entity_manager.map = map;

    return {entity_manager};
}