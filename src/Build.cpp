#include "Build.h"
#include "Window.h"
#include "Render.h"
#include "Input.h"
#include "MessageBus.h"
#include <algorithm>

Build::Manager::Manager() : state(Build::IDLE){};

void Build::Manager::update(ECS::Map *map, double ts)
{
    if (this->state == Build::WAITING_TO_PLACE_STRUCTURE)
    {
        V2 build_dimensions = {2, 2};
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            this->save_structure_placement(map);
            return;
        }
        else if (Input::is_input_active(Input::RIGHT_MOUSE_JUST_PRESSED) || Input::is_input_active(Input::Q_KEY_DOWN))
        {
            this->quit_structure_placement();
            return;
        }
        Rect *camera = Window::get_camera();
        V2 grid_position = map->get_mouse_grid_position();

        for (int i = grid_position.x; i < grid_position.x + build_dimensions.x; ++i)
        {
            for (int j = grid_position.y; j < grid_position.y + build_dimensions.y; ++j)
            {
                Color green = {0x00, 0xFF, 0x00, 0x5F};
                Color red = {0xBB, 0x0A, 0x1E, 0x5F};
                Rect rect = {(i * map->cell_size) - camera->x, (j * map->cell_size) - camera->y, map->cell_size, map->cell_size};
                if (!map->grid[i][j].has_entity)
                {
                    Render::render_rectangle(Render::Layer::WORLD_LAYER, rect, red, true, 2);
                }
                else
                {
                    Render::render_rectangle(Render::Layer::WORLD_LAYER, rect, green, true, 2);
                }
            }
        }
    }
    else if (this->state == Build::WAITING_TO_BUILD_FLOOR)
    {
        if (Input::is_input_active(Input::RIGHT_MOUSE_JUST_PRESSED) || Input::is_input_active(Input::Q_KEY_DOWN))
        {
            this->quit_floor_placement();
            return;
        }
        Color color = {0xFF, 0xFF, 0xFF, 0x4F};
        Render::render_rectangle(Render::Layer::WORLD_LAYER, map->get_hovered_grid_cell(), color, true, 3);
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            this->state = Build::PLACING_FLOOR;
            this->start_floor_grid_position = map->get_mouse_grid_position();
        }
    }
    else if (this->state == Build::PLACING_FLOOR)
    {
        if (!Input::is_input_active(Input::LEFT_MOUSE_PRESSED))
        {
            this->save_floor_placement(map);
            return;
        }
        V2 current_mouse_grid_position = map->get_mouse_grid_position();
        int start_x = std::min(this->start_floor_grid_position.x, current_mouse_grid_position.x);
        int end_x = std::max(this->start_floor_grid_position.x, current_mouse_grid_position.x);
        int start_y = std::min(this->start_floor_grid_position.y, current_mouse_grid_position.y);
        int end_y = std::max(this->start_floor_grid_position.y, current_mouse_grid_position.y);
        Rect *camera = Window::get_camera();
        if (start_x >= 0 && end_x < map->dimensions.x && start_y >= 0 && end_y < map->dimensions.y)
        {
            for (int i = start_x; i <= end_x; ++i)
            {
                for (int j = start_y; j <= end_y; ++j)
                {
                    if (map->grid[i][j].tile.empty)
                    {
                        Rect cell_to_render = {
                            (static_cast<int>(i) * map->cell_size) - camera->x,
                            (static_cast<int>(j) * map->cell_size) - camera->y,
                            map->cell_size,
                            map->cell_size};
                        Color color = {0xFF, 0xFF, 0xFF, 0x6F};
                        Render::render_rectangle(Render::Layer::WORLD_LAYER, cell_to_render, color, true, 1);
                    }
                }
            }
        }
    }
};
void Build::Manager::begin_structure_placement()
{
    this->state = Build::WAITING_TO_PLACE_STRUCTURE;
};
void Build::Manager::begin_floor_placement()
{
    this->state = Build::WAITING_TO_BUILD_FLOOR;
}
void Build::Manager::save_structure_placement(ECS::Map *)
{
    this->state = Build::WAITING_TO_PLACE_STRUCTURE;
};
void Build::Manager::save_floor_placement(ECS::Map *map)
{
    // TODO: consolidate similar code in Build::Manager::update
    this->state = Build::WAITING_TO_BUILD_FLOOR;
    V2 current_mouse_grid_position = map->get_mouse_grid_position();
    int start_x = std::min(this->start_floor_grid_position.x, current_mouse_grid_position.x);
    int end_x = std::max(this->start_floor_grid_position.x, current_mouse_grid_position.x);
    int start_y = std::min(this->start_floor_grid_position.y, current_mouse_grid_position.y);
    int end_y = std::max(this->start_floor_grid_position.y, current_mouse_grid_position.y);
    if (start_x >= 0 && end_x < map->dimensions.x && start_y >= 0 && end_y < map->dimensions.y)
    {
        for (int i = start_x; i <= end_x; ++i)
        {
            for (int j = start_y; j <= end_y; ++j)
            {
                if (map->grid[i][j].tile.empty)
                {
                    MBus::Message message;
                    message.type = MBus::CREATE_TILE;
                    message.data.ct.grid_position = {static_cast<int>(i), static_cast<int>(j)};
                    MBus::send_ecs_message(&message);
                }
            }
        }
    }
};
void Build::Manager::quit_structure_placement()
{
    this->state = Build::IDLE;
};
void Build::Manager::quit_floor_placement()
{
    this->state = Build::IDLE;
};