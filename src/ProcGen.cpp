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
    // We are only placing trees right now:
    int num_trees = rand() % rules->tree_weight;
    num_trees = 200;

    // Instantiate
    std::vector<int> trees;
    std::vector<int> ground;
    int texture_index = Assets::get_texture_index("tilesheet-transparent");
    for (int i = 0; i < num_trees; ++i)
    {
        ECS::Entity tree_entity;
        ECS::Component render_component;
        render_component.type = ECS::Type::RENDER;
        Rect clip = {};
        if (rand() % 5 == 0)
        {
            clip = {128, 80, 16, 32}; // tree
        }
        else if (rand() % 2 == 0)
        {
            clip = {144, 96, 16, 16};
        }
        else if (rand() % 2 == 0)
        {
            clip = {144, 80, 16, 16};
        }
        else if (rand() % 2 == 0)
        {
            clip = {160, 96, 16, 16};
        }
        else if (rand() % 2 == 0)
        {
            clip = {160, 80, 16, 16};
        }
        else if (rand() % 2 == 0)
        {
            clip = {160, 112, 16, 16};
        }
        else
        {
            clip = {144, 80, 16, 16};
        }
        render_component.data.r = {
            clip,
            Render::Layer::WORLD_LAYER,
            texture_index,
            1,
            1,
            true};
        tree_entity.components[render_component.type] = render_component;
        entity_manager.entities.push_back(tree_entity);
        trees.push_back(entity_manager.entities.size() - 1);
    }

    // Placement
    for (unsigned int i = 0; i < trees.size(); ++i)
    {
        bool placed = false;
        while (!placed)
        {
            int x = rand() % dimensions->x;
            int y = rand() % dimensions->y;
            if (map.grid[x][y].entity_id == -1 && y > 1 && map.grid[x][y - 1].entity_id == -1)
            {
                ECS::Entity *entity = &entity_manager.entities[trees[i]];
                ECS::Component position;
                position.type = ECS::Type::POSITION;
                position.data.p.position = {x * 16, (y * 16) - (entity->components[ECS::RENDER].data.r.clip.h - 16)};
                entity->components[position.type] = position;
                map.grid[x][y].entity_id = trees[i];
                placed = true;
            }
        }
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
    map.cell_size = 16;
    map.pixel_dimensions = {map.dimensions.x * map.cell_size, map.dimensions.y * map.cell_size};
    for (int i = 0; i < dimensions->x; ++i)
    {
        for (int j = 0; j < dimensions->y; ++j)
        {
            ECS::Tile t;
            t.texture_index = Assets::get_texture_index("tilesheet-colored");
            t.position = {i * map.cell_size, j * map.cell_size};
            if (rand() % 25 == 0)
            {
                if (rand() % 2 == 0)
                {
                    t.clip = {128, 0, 16, 16};
                }
                else if (rand() % 2 == 0)
                {
                    t.clip = {128, 16, 16, 16};
                }
                else
                {
                    t.clip = {112, 0, 16, 16};
                }
            }
            else
            {
                t.clip = {85, 34, 16, 16};
            }
            map.grid[i][j].tile = t;
        }
    }
    entity_manager.map = map;

    return {entity_manager};
}