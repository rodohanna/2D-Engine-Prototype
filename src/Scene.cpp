#include "Scene.h"

#include "MapGen.h"
#include "GameTypes.h"
#include "Camera.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>

Scene::Scene(EventBus *e) : event_bus(e)
{
    V2 world_dimensions = {5, 5};
    Palette p = MapGen::load_palette("assets/palette.txt");
    World world(p, world_dimensions, 64);
    world.generate_world();
    this->chunk_manager = std::make_unique<ChunkManager>(ChunkManager(e, world, 64));
    V2 player = {0, 0};
    this->chunk_manager.get()->sync_chunks_to_world_position(player);
};

void Scene::update(double ts)
{
    Rect camera = Camera::get_camera();
    V2 player = {camera.x, camera.y};
    this->chunk_manager.get()->sync_chunks_to_world_position(player);
    this->chunk_manager.get()->update_chunks(ts);
}