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
    for (int i = 0; i < map->dimensions.x; ++i)
    {
        for (int j = 0; j < map->dimensions.y; ++j)
        {
            ECS::Tile tile = map->grid[i][j].tile;
            picojson::object tile_object;
            if (!tile.empty)
            {
                picojson::object clip;
                picojson::array tile_entity_components_array;
                for (int i = 0; i < tile.tile_entity.component_length; ++i)
                {
                    picojson::object component_object = ECS::jsonize_component(
                        tile.tile_entity.components[i].type,
                        &tile.tile_entity.components[i]);
                    tile_entity_components_array.push_back(picojson::value(component_object));
                }
                tile_object["grid_x"] = picojson::value((double)i);
                tile_object["grid_y"] = picojson::value((double)j);
                tile_object["component_flags"] = picojson::value((double)tile.tile_entity.component_flags);
                tile_object["tile_components"] = picojson::value(tile_entity_components_array);
                if (map->grid[i][j].has_entity)
                {
                    tile_object["entity_id"] = picojson::value((double)map->grid[i][j].entity_id);
                }
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
        entity_object["component_flags"] = picojson::value((double)entity->component_flags);
        for (int i = 0; i < entity->component_length; ++i)
        {
            picojson::object component_object = ECS::jsonize_component(entity->components[i].type, &entity->components[i]);
            components_array.push_back(picojson::value(component_object));
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
    picojson::value entities_value = save_object.get<picojson::object>()["entities"];
    picojson::value dimensions_value = save_object.get<picojson::object>()["dimensions"];
    if (!tiles_value.is<picojson::array>())
    {
        printf("Load JSON Err: Tiles field is not array\n");
        return result;
    }
    if (!entities_value.is<picojson::array>())
    {
        printf("Load JSON Err: Entities field is not array\n");
        return result;
    }
    if (!dimensions_value.is<picojson::object>())
    {
        printf("Load JSON Err: Dimensions field is not object\n");
        return result;
    }
    picojson::array tiles_array = tiles_value.get<picojson::array>();
    picojson::array entities_array = entities_value.get<picojson::array>();
    picojson::object dimensions_object = dimensions_value.get<picojson::object>();

    // ************* DIMENSIONS *****************
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

    int cell_size = 32;
    result.entity_manager.map.grid = new ECS::Cell *[dimensions.x];
    for (int i = 0; i < dimensions.x; ++i)
    {
        result.entity_manager.map.grid[i] = new ECS::Cell[dimensions.y];
    }
    for (int i = 0; i < dimensions.x; ++i)
    {
        for (int j = 0; j < dimensions.y; ++j)
        {
            result.entity_manager.map.grid[i][j].has_entity = false;
            result.entity_manager.map.grid[i][j].tile.empty = true;
        }
    }
    result.entity_manager.map.dimensions = dimensions;
    result.entity_manager.map.cell_size = cell_size;
    result.entity_manager.map.pixel_dimensions = {dimensions.x * cell_size, dimensions.y * cell_size};
    // ******************************************

    // ************* TILES *****************
    for (picojson::value::array::const_iterator obj_it = tiles_array.begin(); obj_it != tiles_array.end(); ++obj_it)
    {
        if (obj_it->is<picojson::object>())
        {
            picojson::object tile_object = obj_it->get<picojson::object>();
            if (tile_object.find("grid_x") != tile_object.end() && tile_object.find("grid_y") != tile_object.end() &&
                tile_object.find("tile_components") != tile_object.end() && tile_object.find("component_flags") != tile_object.end())
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
                if (!tile_object["tile_components"].is<picojson::array>())
                {
                    printf("JSON Load Err: Tile 'world_x' value is not double\n");
                    continue;
                }
                if (!tile_object["component_flags"].is<double>())
                {
                    printf("JSON Load Err: Tile 'component_flags' value is not double\n");
                    continue;
                }
                // ************* EVERYTHING IS VALIDATED *************
                int grid_x = static_cast<int>(tile_object["grid_x"].get<double>());
                int grid_y = static_cast<int>(tile_object["grid_y"].get<double>());
                int component_flags = static_cast<int>(tile_object["component_flags"].get<double>());
                picojson::array tile_components_array = tile_object["tile_components"].get<picojson::array>();

                if (grid_x >= 0 && grid_x < result.entity_manager.map.dimensions.x &&
                    grid_y >= 0 && grid_y < result.entity_manager.map.dimensions.y)
                {
                    ECS::Entity tile_entity;
                    tile_entity.component_flags = component_flags;
                    for (picojson::value::array::const_iterator component_obj_it = tile_components_array.begin();
                         component_obj_it != tile_components_array.end();
                         ++component_obj_it)
                    {
                        if (component_obj_it->is<picojson::object>())
                        {
                            picojson::object component_object = component_obj_it->get<picojson::object>();
                            ECS::ComponentizeJsonResult cjr = ECS::componentize_json(&component_object);
                            if (cjr.success)
                            {
                                tile_entity.add_component(&cjr.component);
                            }
                            else
                            {
                                printf("JSON Load Err: componentize_json failed\n");
                            }
                        }
                        else
                        {
                            printf("JSON Load Err: Entity 'components' array contains non-object value\n");
                        }
                    }
                    result.entity_manager.map.grid[grid_x][grid_y].tile.tile_entity = tile_entity;
                    result.entity_manager.map.grid[grid_x][grid_y].tile.empty = false;
                    if (tile_object["entity_id"].is<double>())
                    {
                        result.entity_manager.map.grid[grid_x][grid_y].has_entity = true;
                        result.entity_manager.map.grid[grid_x][grid_y].entity_id = static_cast<int>(tile_object["entity_id"].get<double>());
                    }
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
    // ******************************************

    std::vector<ECS::Entity> entities_array_return(entities_array.size());
    // ************** ENTITIES ******************
    for (picojson::value::array::const_iterator obj_it = entities_array.begin(); obj_it != entities_array.end(); ++obj_it)
    {
        if (obj_it->is<picojson::object>())
        {
            ECS::Entity entity;
            picojson::object entity_object = obj_it->get<picojson::object>();
            if (entity_object.find("id") != entity_object.end() && entity_object.find("components") != entity_object.end() &&
                entity_object.find("component_flags") != entity_object.end())
            {
                // ***************** VALIDATION *****************
                if (!entity_object["id"].is<double>())
                {
                    printf("JSON Load Err: Entity 'id' value is not double\n");
                    continue;
                }
                if (!entity_object["components"].is<picojson::array>())
                {
                    printf("JSON Load Err: Entity 'components' value is not array\n");
                    continue;
                }
                if (!entity_object["component_flags"].is<double>())
                {
                    printf("JSON Load Err: Entity 'component_flags' value is not double\n");
                    continue;
                }
                picojson::array component_array = entity_object["components"].get<picojson::array>();
                for (picojson::value::array::const_iterator component_it = component_array.begin(); component_it != component_array.end(); ++component_it)
                {
                    if (component_it->is<picojson::object>())
                    {
                        picojson::object component_object = component_it->get<picojson::object>();
                        ECS::ComponentizeJsonResult cjr = ECS::componentize_json(&component_object);
                        if (cjr.success)
                        {
                            entity.add_component(&cjr.component);
                        }
                        else
                        {
                            printf("JSON Load Err: componentize_json failed\n");
                        }
                    }
                    else
                    {
                        printf("JSON Load Err: Entity 'components' array contains non-object value\n");
                    }
                }
                int id = static_cast<int>(entity_object["id"].get<double>());
                int component_flags = static_cast<int>(entity_object["component_flags"].get<double>());
                entity.component_flags = component_flags;
                entities_array_return[id] = entity;
            }
            else
            {
                printf("JSON Load Err: Entity is missing required fields\n");
            }
        }
        else
        {
            printf("Load JSON Err: Entities should be an array of objects\n");
        }
    }
    result.entity_manager.entities = entities_array_return;
    // ******************************************
    result.success = true;
    return result;
}