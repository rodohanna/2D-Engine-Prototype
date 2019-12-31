#include "Build.h"
#include "Window.h"
#include "Render.h"

Build::Manager::Manager() : placing_structure(false){};

void Build::Manager::update(ECS::Map *map, double ts)
{
    if (this->placing_structure)
    {
        Rect *camera = Window::get_camera();
        V2 *mouse_position = Window::get_mouse_position();
        int cell_size = map->cell_size;
        V2 world_mouse_position = {mouse_position->x + camera->x, mouse_position->y + camera->y};
        V2 grid_position = {
            world_mouse_position.x / cell_size,
            world_mouse_position.y / cell_size};

        for (int i = grid_position.x; i < grid_position.x + this->build_dimensions.x; ++i)
        {
            for (int j = grid_position.y; j < grid_position.y + this->build_dimensions.y; ++j)
            {
                Color green = {0x00, 0xFF, 0x00, 0x5F};
                Color red = {0xBB, 0x0A, 0x1E, 0x5F};
                Rect rect = {i * map->cell_size, j * map->cell_size, map->cell_size, map->cell_size};
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
    this->placing_structure = true;
};
void Build::Manager::quit_and_save_structure_placement(ECS::Map *)
{
    this->placing_structure = false;
};
void Build::Manager::quit_structure_placement()
{
    this->placing_structure = false;
};