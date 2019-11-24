#include "MapGen.h"

#include "Scenery.h"
#include "Assets.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>

Palette MapGen::load_palette(std::string path)
{
    Palette palette = {};
    palette.scale = 1;
    std::ifstream palette_manifest(path);
    if (palette_manifest.fail())
    {
        palette_manifest.close();
        printf("Error: Unable to load palette file at %s\n", path.c_str());
        return palette;
    }
    std::string type, key, texture_key;
    size_t x_offset, y_offset, width, height;
    while (palette_manifest >> type >> key >> texture_key >> x_offset >> y_offset >> width >> height)
    {
        if (palette_manifest.fail())
        {
            printf("Reading palette %s failed, quiting early\n", path.c_str());
            return palette;
        }
        int texture_index = Assets::get_texture_index(texture_key);
        if (texture_index == -1)
        {
            printf("Could not find texture index for texture_key %s\n", texture_key.c_str());
        }
        Tile t = {
            {static_cast<int>(x_offset),
             static_cast<int>(y_offset),
             static_cast<int>(width),
             static_cast<int>(height)},
            static_cast<size_t>(texture_index),
            palette.scale};
        if (type == "background")
        {
            palette.background_tile = t;
        }
        else if (type == "tree")
        {
            palette.tree_tiles.push_back(t);
        }
        else if (type == "ground")
        {
            palette.ground_tiles.push_back(t);
        }
    }
    return palette;
}

Map MapGen::generate_map(Palette *p, ProcGenRules *r, EventBus *e, const V2 &dimensions, const V2 &world_offset)
{
    Map map;
    for (int i = 0; i < dimensions.x; ++i)
    {
        std::vector<std::shared_ptr<IEntity>> column;
        map.grid.push_back(column);
    }
    BackGround *ground = new BackGround(e, p->background_tile.clip, {0, 0}, p->background_tile.texture_index);
    map.background = std::shared_ptr<IEntity>(ground);
    for (int i = 0; i < dimensions.x; ++i)
    {
        for (int j = 0; j < dimensions.y; ++j)
        {
            if (static_cast<size_t>((rand() % 100)) < r->tree_weight)
            {
                size_t rand_index = rand() % p->tree_tiles.size();
                size_t texture_index = p->tree_tiles[rand_index].texture_index;
                Rect clip = p->tree_tiles[rand_index].clip;
                size_t scale = p->tree_tiles[rand_index].scale;
                Tree *tree = new Tree(e, clip, {(world_offset.x * 16) + (i * 16), (world_offset.y * 16) + (j * 16)}, texture_index, scale);
                map.grid[i].push_back(std::shared_ptr<IEntity>(tree));
            }
            else if (static_cast<size_t>((rand() % 100)) < r->ground_weight)
            {
                size_t rand_index = rand() % p->ground_tiles.size();
                size_t texture_index = p->ground_tiles[rand_index].texture_index;
                Rect clip = p->ground_tiles[rand_index].clip;
                size_t scale = p->ground_tiles[rand_index].scale;
                Dirt *dirt = new Dirt(e, clip, {(world_offset.x * 16) + (i * 16), (world_offset.y * 16) + (j * 16)}, texture_index, scale);
                map.grid[i].push_back(std::shared_ptr<IEntity>(dirt));
            }
            else
            {
                map.grid[i].push_back(nullptr);
            }
        }
    }
    return map;
}