#include "Scene.h"

#include "MapGen.h"
#include "GameTypes.h"
#include "Window.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>

Scene::Scene(EventBus *e) : event_bus(e)
{
    V2 world_dimensions = {100, 100};
    Palette p = MapGen::load_palette("assets/palette.txt");
    World world(p, world_dimensions, 64);
    world.generate_world();
    ChunkManager *c_m = new ChunkManager(e, world, 64);
    this->chunk_manager = std::unique_ptr<ChunkManager>(c_m);
    V2 player = {0, 0};
    this->chunk_manager->sync_chunks_to_world_position(player, true);
    this->gui = std::make_unique<GUI>(GUI(e));
    Console *c = new Console(e);
    this->console = std::unique_ptr<Console>(c);
};

void Scene::update(double ts)
{
    V2 player = {Window::get_camera()->x, Window::get_camera()->y};
    this->gui->update(ts);
    this->chunk_manager->sync_chunks_to_world_position(player);
    this->chunk_manager->update_chunks(ts);
    this->console->update(ts);
}