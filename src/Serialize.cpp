#include "Serialize.h"

#include "json/picojson.h"
#include "GameTypes.h"
#include "Assets.h"
#include <vector>
#include <fstream>

bool Serialize::save_game(ECS::Manager *entity_manager, std::string file)
{
    ECS::Map *map = &entity_manager->map;
    picojson::object save_object;
    picojson::array tiles;
    picojson::value::object dimensions_object;
    picojson::value::array entity_array;

    // Serialize Map
    for (unsigned int i = 0; i < map->grid.size(); ++i)
    {
        for (unsigned int j = 0; j < map->grid[i].size(); ++j)
        {
            ECS::Tile tile = map->grid[i][j].tile;
            picojson::object tile_object;
            if (!tile.empty)
            {
                picojson::object clip;
                tile_object["grid_x"] = picojson::value((double)tile.grid_position.x);
                tile_object["grid_y"] = picojson::value((double)tile.grid_position.y);
                tile_object["world_x"] = picojson::value((double)tile.world_position.x);
                tile_object["world_y"] = picojson::value((double)tile.world_position.y);
                tile_object["texture_key"] = picojson::value(tile.texture_key);
                clip["x"] = picojson::value((double)tile.clip.x);
                clip["y"] = picojson::value((double)tile.clip.y);
                clip["w"] = picojson::value((double)tile.clip.w);
                clip["h"] = picojson::value((double)tile.clip.h);
                tile_object["clip"] = picojson::value(clip);
            }
            if (map->grid[i][j].has_entity)
            {
                tile_object["entity_id"] = picojson::value((double)map->grid[i][j].entity_id);
            }

            if (!tile.empty || map->grid[i][j].has_entity)
            {
                tiles.push_back(picojson::value(tile_object));
            }
        }
    }

    // Serialize Entities
    for (unsigned int i = 0; i < entity_manager->entities.size(); ++i)
    {
        ECS::Entity *entity = &entity_manager->entities[i];
        picojson::object entity_object;
        picojson::array components_array;
        entity_object["id"] = picojson::value((double)i);

        for (auto component_it = entity->components.begin(); component_it != entity->components.end(); ++component_it)
        {
            printf("Looking at component: %d\n", component_it->first);
            picojson::object compoment_object = ECS::jsonize_component(component_it->first, &component_it->second);
            components_array.push_back(picojson::value(compoment_object));
        }
        entity_object["components"] = picojson::value(components_array);
        entity_array.push_back(picojson::value(entity_object));
    }

    dimensions_object["x"] = picojson::value((double)map->dimensions.x);
    dimensions_object["y"] = picojson::value((double)map->dimensions.y);
    save_object["dimensions"] = picojson::value(dimensions_object);
    save_object["tiles"] = picojson::value(tiles);
    save_object["entities"] = picojson::value(entity_array);
    picojson::value save_value = picojson::value(save_object);

    printf("%s\n", save_value.serialize().c_str());
    std::ofstream save_file(file);
    if (save_file.is_open())
    {
        save_file << save_value.serialize(true); // DEBUG
        save_file.close();
        return true;
    }
    else
    {
        printf("Error: could not save map!s\n");
        return false;
    }
};

Serialize::LoadMapResult Serialize::load_game(std::string file)
{
    Serialize::LoadMapResult result;
    result.success = false;
    std::ifstream save_file(file);
    if (!save_file.is_open())
    {
        return result;
    }
    picojson::value save_object;
    save_file >> save_object;
    std::string err = picojson::get_last_error();
    if (err.size() > 0)
    {
        printf("Error: could not load save: %s\n", err.c_str());
        return result;
    }

    if (!save_object.is<picojson::object>())
    {
        printf("Load JSON Err: Save object root is not object\n");
        return result;
    }

    picojson::value tiles_value = save_object.get<picojson::object>()["tiles"];
    picojson::value dimensions_value = save_object.get<picojson::object>()["dimensions"];
    if (!tiles_value.is<picojson::array>())
    {
        printf("Load JSON Err: Tiles field is not array\n");
        return result;
    }
    if (!dimensions_value.is<picojson::object>())
    {
        printf("Load JSON Err: Dimensions field is not object\n");
        return result;
    }
    picojson::value::array tiles_array = tiles_value.get<picojson::array>();
    picojson::object dimensions_object = dimensions_value.get<picojson::object>();
    V2 dimensions = {};
    if (dimensions_object.find("x") != dimensions_object.end() && dimensions_object.find("y") != dimensions_object.end())
    {
        if (!dimensions_object["x"].is<double>())
        {
            printf("JSON Load Err: Dimensions 'x' value is not double\n");
            return result;
        }
        if (!dimensions_object["y"].is<double>())
        {
            printf("JSON Load Err: Dimensions 'y' value is not double\n");
            return result;
        }
        dimensions.x = dimensions_object["x"].get<double>();
        dimensions.y = dimensions_object["y"].get<double>();
    }
    else
    {
        printf("Load JSON Err: Dimensions field is not object\n");
        return result;
    }

    // TODO, USE DIMENSIONS THAT ARE BEING SAVED.
    int cell_size = 32;
    for (int i = 0; i < dimensions.x; ++i)
    {
        std::vector<ECS::Cell> column;
        for (int j = 0; j < dimensions.y; ++j)
        {
            column.push_back({-1});
            column[j].tile.empty = true;
            column[j].tile.world_position = {i * cell_size, j * cell_size};
            column[j].tile.grid_position = {i, j};
        }
        result.entity_manager.map.grid.push_back(column);
    }
    result.entity_manager.map.dimensions = dimensions;
    result.entity_manager.map.cell_size = cell_size;
    result.entity_manager.map.pixel_dimensions = {dimensions.x * cell_size, dimensions.y * cell_size};

    for (picojson::value::array::const_iterator obj_it = tiles_array.begin(); obj_it != tiles_array.end(); ++obj_it)
    {
        if (obj_it->is<picojson::object>())
        {
            picojson::object tile_object = obj_it->get<picojson::object>();
            if (tile_object.find("grid_x") != tile_object.end() && tile_object.find("grid_y") != tile_object.end() &&
                tile_object.find("world_x") != tile_object.end() && tile_object.find("world_y") != tile_object.end() &&
                tile_object.find("texture_key") != tile_object.end() && tile_object.find("clip") != tile_object.end())
            {
                // ***************** VALIDATION *****************
                if (!tile_object["grid_x"].is<double>())
                {
                    printf("JSON Load Err: Tile 'grid_x' value is not double\n");
                    continue;
                }
                if (!tile_object["grid_y"].is<double>())
                {
                    printf("JSON Load Err: Tile 'grid_y' value is not double\n");
                    continue;
                }
                if (!tile_object["world_x"].is<double>())
                {
                    printf("JSON Load Err: Tile 'world_x' value is not double\n");
                    continue;
                }
                if (!tile_object["world_y"].is<double>())
                {
                    printf("JSON Load Err: Tile 'world_y' value is not double\n");
                    continue;
                }
                if (!tile_object["texture_key"].is<std::string>())
                {
                    printf("JSON Load Err: Tile 'texture_key' value is not string\n");
                    continue;
                }
                if (!tile_object["clip"].is<picojson::object>())
                {
                    printf("JSON Load Err: Tile 'clip' value is not a picojson::object\n");
                    continue;
                }
                picojson::object clip = tile_object["clip"].get<picojson::object>();
                if (clip.find("x") != clip.end() && clip.find("y") != clip.end() &&
                    clip.find("w") != clip.end() && clip.find("h") != clip.end())
                {
                    if (!clip["x"].is<double>())
                    {
                        printf("JSON Load Err: Tile 'clip.x' value is not a double\n");
                        continue;
                    }
                    if (!clip["y"].is<double>())
                    {
                        printf("JSON Load Err: Tile 'clip.y' value is not a double\n");
                        continue;
                    }
                    if (!clip["w"].is<double>())
                    {
                        printf("JSON Load Err: Tile 'clip.w' value is not a double\n");
                        continue;
                    }
                    if (!clip["h"].is<double>())
                    {
                        printf("JSON Load Err: Tile 'clip.h' value is not a double\n");
                        continue;
                    }
                }
                else
                {
                    printf("JSON Load Err: Tile 'clip' is missing field\n");
                    continue;
                }
                // ************* EVERYTHING IS VALIDATED *************
                int grid_x = static_cast<int>(tile_object["grid_x"].get<double>());
                int grid_y = static_cast<int>(tile_object["grid_y"].get<double>());
                int world_x = static_cast<int>(tile_object["world_x"].get<double>());
                int world_y = static_cast<int>(tile_object["world_y"].get<double>());
                std::string texture_key = tile_object["texture_key"].get<std::string>();
                int clip_x = static_cast<int>(clip["x"].get<double>());
                int clip_y = static_cast<int>(clip["y"].get<double>());
                int clip_w = static_cast<int>(clip["w"].get<double>());
                int clip_h = static_cast<int>(clip["h"].get<double>());

                if (grid_x >= 0 && grid_x < result.entity_manager.map.dimensions.x &&
                    grid_y >= 0 && grid_y < result.entity_manager.map.dimensions.y)
                {
                    result.entity_manager.map.grid[grid_x][grid_y].tile.grid_position = {grid_x, grid_y};
                    result.entity_manager.map.grid[grid_x][grid_y].tile.world_position = {world_x, world_y};
                    result.entity_manager.map.grid[grid_x][grid_y].tile.clip = {clip_x, clip_y, clip_w, clip_h};
                    result.entity_manager.map.grid[grid_x][grid_y].tile.texture_key = texture_key;
                    result.entity_manager.map.grid[grid_x][grid_y].tile.texture_index = Assets::get_texture_index(texture_key);
                    result.entity_manager.map.grid[grid_x][grid_y].tile.empty = false;
                    // printf("Successfully loaded tile at: %d %d with texture key: %s and clip vals: %d %d %d %d\n",
                    //        grid_x, grid_y, texture_key.c_str(), clip_x, clip_y, clip_w, clip_h);
                }
                else
                {
                    printf("JSON Load Err: Invalid grid_x or grid_y position for tile: %d %d\n", grid_x, grid_y);
                }
            }
            else
            {
                printf("JSON Load Err: Tile is missing required fields\n");
            }
        }
        else
        {
            printf("Load JSON Err: Tiles should be an array of objects\n");
        }
    }
    result.success = true;
    return result;
}