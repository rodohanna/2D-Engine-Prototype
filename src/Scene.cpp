#include "Scene.h"

#include "MapGen.h"
#include "GameTypes.h"
#include <sstream>
#include <fstream>

Scene::Scene(EventBus *e) : event_bus(e){};

void Scene::load_from_file(std::string path)
{
    std::ifstream scene(path);
    std::string name, palette, type;
    V2 dimensions = {};
    size_t weight = 0;
    int seed = 0;
    ProcGenRules rules = {};
    scene >> name >> seed >> palette >> dimensions.x >> dimensions.y;
    while (scene >> type >> weight)
    {
        if (scene.fail())
        {
            printf("Reading scene %s failed, quiting early\n", path.c_str());
            return;
        }
        if (type == "tree")
        {
            rules.tree_weight = weight;
        }
        else if (type == "ground")
        {
            rules.ground_weight = weight;
        }
    }
    Palette p = MapGen::load_palette(palette);
    this->map = MapGen::generate_map(&p, &rules, this->event_bus, dimensions);
}

void Scene::update(double ts)
{
    for (size_t i = 0; i < this->map.size(); ++i)
    {
        this->map[i].get()->update(ts);
    }
}