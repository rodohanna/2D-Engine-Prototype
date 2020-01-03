#include "Build.h"
#include "Window.h"
#include "Render.h"
#include "Input.h"

Build::Manager::Manager() : state(Build::IDLE){};

void Build::Manager::update(ECS::Map *map, double ts)
{
    if (this->state == Build::WAITING_TO_PLACE_STRUCTURE)
    {
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

        for (int i = grid_position.x; i < grid_position.x + this->build_dimensions.x; ++i)
        {
            for (int j = grid_position.y; j < grid_position.y + this->build_dimensions.y; ++j)
            {
                Color green = {0x00, 0xFF, 0x00, 0x5F};
                Color red = {0xBB, 0x0A, 0x1E, 0x5F};
                Rect rect = {(i * map->cell_size) - camera->x, (j * map->cell_size) - camera->y, map->cell_size, map->cell_size};
                if (map->grid[i][j].entity_id != -1)
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
};
void Build::Manager::begin_structure_placement(V2 *build_dimensions)
{
    this->build_dimensions = *build_dimensions;
    this->state = Build::WAITING_TO_PLACE_STRUCTURE;
};
void Build::Manager::save_structure_placement(ECS::Map *)
{
    this->state = Build::WAITING_TO_PLACE_STRUCTURE;
};
void Build::Manager::quit_structure_placement()
{
    this->state = Build::IDLE;
};