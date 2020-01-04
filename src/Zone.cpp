#include "Zone.h"
#include "Window.h"
#include "Render.h"
#include "MessageBus.h"
#include "Input.h"
#include <algorithm>
#include <stdio.h>

Zone::Manager::Manager() : state(Zone::IDLE){};

void Zone::Manager::update(ECS::Map *map, double ts)
{
    if (this->state == Zone::PLACING_ZONE)
    {
        if (!Input::is_input_active(Input::LEFT_MOUSE_PRESSED))
        {
            this->save_zone_placement(map);
            return;
        }
        V2 current_mouse_grid_position = map->get_mouse_grid_position();
        unsigned int start_x = std::min(this->start_zone_grid_position.x, current_mouse_grid_position.x);
        unsigned int end_x = std::max(this->start_zone_grid_position.x, current_mouse_grid_position.x);
        unsigned int start_y = std::min(this->start_zone_grid_position.y, current_mouse_grid_position.y);
        unsigned int end_y = std::max(this->start_zone_grid_position.y, current_mouse_grid_position.y);
        Rect *camera = Window::get_camera();
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
                        Color color = {0xFF, 0xFF, 0xFF, 0x6F};
                        Render::render_rectangle(Render::Layer::WORLD_LAYER, cell_to_render, color, true, 2);
                    }
                }
            }
        }
    }
    else if (this->state == Zone::WAITING_TO_PLACE_ZONE)
    {
        if (Input::is_input_active(Input::RIGHT_MOUSE_JUST_PRESSED) || Input::is_input_active(Input::Q_KEY_DOWN))
        {
            this->quit_zone_placement();
            return;
        }
        Color color = {0xFF, 0xFF, 0xFF, 0x4F};
        Render::render_rectangle(Render::Layer::WORLD_LAYER, map->get_hovered_grid_cell(), color, true, 3);
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            this->state = Zone::PLACING_ZONE;
            this->start_zone_grid_position = map->get_mouse_grid_position();
        }
    }
};
void Zone::Manager::wait_for_zone_placement(ECS::Map *map)
{
    this->state = Zone::WAITING_TO_PLACE_ZONE;
}
void Zone::Manager::quit_zone_placement() { this->state = Zone::IDLE; }
void Zone::Manager::save_zone_placement(ECS::Map *map)
{
    // TODO: consolidate similar code in Zone::Manager::update
    this->state = Zone::WAITING_TO_PLACE_ZONE;
    V2 current_mouse_grid_position = map->get_mouse_grid_position();
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