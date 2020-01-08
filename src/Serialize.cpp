#include "Serialize.h"

#include "json/picojson.h"
#include <vector>

void Serialize::save_map(ECS::Map *map)
{
    picojson::value save_object = picojson::value(picojson::object());

    picojson::value tiles = picojson::value(std::vector<picojson::value>());
    save_object.get<picojson::object>()["tiles"] = tiles;

    for (unsigned int i = 0; i < map->grid.size(); ++i)
    {
        for (unsigned int j = 0; j < map->grid[i].size(); ++j)
        {
            ECS::Tile tile = map->grid[i][j].tile;
            if (!tile.empty)
            {
                picojson::object tile_object;
                tile_object["x"] = picojson::value((double)tile.position.x);
                tile_object["y"] = picojson::value((double)tile.position.y);
                save_object.get<picojson::object>()["tiles"].get<picojson::array>().push_back(picojson::value(tile_object));
            }
        }
    }
    printf("%s\n", save_object.serialize().c_str());
};