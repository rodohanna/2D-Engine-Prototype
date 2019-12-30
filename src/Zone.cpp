#include "Zone.h"
#include "Window.h"
#include "Render.h"
#include "MessageBus.h"
#include <algorithm>
#include <stdio.h>

Zone::Manager::Manager() : placing_zone(false){};

void Zone::Manager::update(ECS::Map *map, double ts)
{
    if (this->placing_zone)
    {
        Rect *camera = Window::get_camera();
        V2 *mouse_position = Window::get_mouse_position();
        V2 world_mouse_position = {mouse_position->x + camera->x, mouse_position->y + camera->y};
        V2 current_mouse_grid_position = {
            world_mouse_position.x / map->cell_size,
            world_mouse_position.y / map->cell_size};
        unsigned int start_x = std::min(this->start_zone_grid_position.x, current_mouse_grid_position.x);
        unsigned int end_x = std::max(this->start_zone_grid_position.x, current_mouse_grid_position.x);
        unsigned int start_y = std::min(this->start_zone_grid_position.y, current_mouse_grid_position.y);
        unsigned int end_y = std::max(this->start_zone_grid_position.y, current_mouse_grid_position.y);
        if (start_x >= 0 && end_x < map->grid.size() && start_y >= 0 && end_y < map->grid.size())
        {
            for (unsigned int i = start_x; i <= end_x; ++i)
            {
                for (unsigned int j = start_y; j <= end_y; ++j)
                {
                    if (map->grid[i][j].entity_id == -1)
                    {
                        Rect cell_to_render = {
                            (static_cast<int>(i) * map->cell_size) - camera->x,
                            (static_cast<int>(j) * map->cell_size) - camera->y,
                            map->cell_size,
                            map->cell_size};
                        Color color = {0xFF, 0xFF, 0xFF, 0x0F};
                        Render::render_rectangle(Render::Layer::WORLD_LAYER, cell_to_render, color, true, 2);
                    }
                }
            }
        }
    }
};
void Zone::Manager::begin_zone_placement(ECS::Map *map)
{
    this->placing_zone = true;
    Rect *camera = Window::get_camera();
    V2 *mouse_position = Window::get_mouse_position();
    int cell_size = map->cell_size;

    V2 world_mouse_position = {mouse_position->x + camera->x, mouse_position->y + camera->y};

    this->start_zone_grid_position = {
        world_mouse_position.x / cell_size,
        world_mouse_position.y / cell_size};
}
void Zone::Manager::quit_zone_placement(ECS::Map *map) { this->placing_zone = false; }
void Zone::Manager::quit_and_save_zone_placement(ECS::Map *map)
{
    // TODO: consolidate similar code in Zone::Manager::update
    this->placing_zone = false;
    Rect *camera = Window::get_camera();
    V2 *mouse_position = Window::get_mouse_position();
    V2 world_mouse_position = {mouse_position->x + camera->x, mouse_position->y + camera->y};
    V2 current_mouse_grid_position = {
        world_mouse_position.x / map->cell_size,
        world_mouse_position.y / map->cell_size};
    unsigned int start_x = std::min(this->start_zone_grid_position.x, current_mouse_grid_position.x);
    unsigned int end_x = std::max(this->start_zone_grid_position.x, current_mouse_grid_position.x);
    unsigned int start_y = std::min(this->start_zone_grid_position.y, current_mouse_grid_position.y);
    unsigned int end_y = std::max(this->start_zone_grid_position.y, current_mouse_grid_position.y);
    if (start_x >= 0 && end_x < map->grid.size() && start_y >= 0 && end_y < map->grid.size())
    {
        for (unsigned int i = start_x; i <= end_x; ++i)
        {
            for (unsigned int j = start_y; j <= end_y; ++j)
            {
                if (map->grid[i][j].entity_id == -1)
                {
                    MBus::Message message;
                    message.type = MBus::CREATE_PLANT_ENTITY;
                    message.data.cpe.grid_position = {static_cast<int>(i), static_cast<int>(j)};
                    MBus::send_ecs_message(&message);
                }
            }
        }
    }
}