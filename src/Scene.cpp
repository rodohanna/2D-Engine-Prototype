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
    this->chunk_manager = std::make_unique<ChunkManager>(ChunkManager(e, world, 64));
    V2 player = {0, 0};
    this->chunk_manager.get()->sync_chunks_to_world_position(player);
    this->gui = std::make_unique<GUI>(GUI(e));
    Console *console = new Console(e);
    this->console = std::unique_ptr<Console>(console);
};

void Scene::update(double ts)
{
    V2 player = {Window::get_camera()->x, Window::get_camera()->y};
    this->gui->update(ts);
    this->chunk_manager.get()->sync_chunks_to_world_position(player);
    this->chunk_manager.get()->update_chunks(ts);
    this->console->update(ts);
}