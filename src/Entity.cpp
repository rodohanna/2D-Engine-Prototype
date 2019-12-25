#include "Entity.h"
#include "Window.h"
#include "Input.h"
#include "Physics.h"
#include <stdio.h>

// Systems

void ECS::render_system(Entity *e)
{
    auto render_it = e->components.find(ECS::Type::RENDER);
    auto position_it = e->components.find(ECS::Type::POSITION);
    if (render_it != e->components.end() && position_it != e->components.end())
    {
        auto render_component = render_it->second.data.r;
        auto position_component = position_it->second.data.p;
        if (!render_component.has_clip)
        {
            render_component.clip = {};
        }
        Rect *camera = Window::get_camera();
        Rect entity_rect = {position_component.position.x, position_component.position.y, 16, 16};
        if (Physics::check_collision(camera, &entity_rect))
        {
            V2 render_position = {position_component.position.x - camera->x, position_component.position.y - camera->y};
            Render::render_texture(
                render_component.layer,
                render_component.texture_index,
                render_component.clip,
                render_position,
                nullptr,
                render_component.scale,
                render_component.z_index);
        }
    }
}

void ECS::camera_system(Entity *e)
{
    auto camera_it = e->components.find(ECS::Type::CAMERA);
    auto position_it = e->components.find(ECS::Type::POSITION);
    if (camera_it != e->components.end() && position_it != e->components.end())
    {
        auto position = position_it->second.data.p.position;
        Window::set_camera_position(position);
    }
}

void ECS::input_system(ECS::Map *map, Entity *e, double ts)
{
    double speed = 200;
    auto player_input_it = e->components.find(ECS::Type::PLAYER_INPUT);
    auto position_it = e->components.find(ECS::Type::POSITION);
    if (player_input_it != e->components.end() && position_it != e->components.end())
    {
        auto w_pressed = Input::is_input_active(Input::Event::W_KEY_DOWN);
        auto a_pressed = Input::is_input_active(Input::Event::A_KEY_DOWN);
        auto s_pressed = Input::is_input_active(Input::Event::S_KEY_DOWN);
        auto d_pressed = Input::is_input_active(Input::Event::D_KEY_DOWN);
        auto position = &position_it->second.data.p.position;
        int vel = speed * ts;
        if (w_pressed)
        {
            position->y -= vel;
        }
        if (s_pressed)
        {
            position->y += vel;
        }
        if (a_pressed)
        {
            position->x -= vel;
        }
        if (d_pressed)
        {
            position->x += vel;
        }

        auto camera = Window::get_camera();

        if (position->x < 0)
        {
            position->x = 0;
        }
        if (position->x + camera->w > map->pixel_dimensions.x)
        {
            position->x = map->pixel_dimensions.x - camera->w;
        }
        if (position->y < 0)
        {
            position->y = 0;
        }
        if (position->y + camera->h > map->pixel_dimensions.y)
        {
            position->y = map->pixel_dimensions.y - camera->h;
        }
        if (camera->w > map->pixel_dimensions.x)
        {
            position->x = -((camera->w - map->pixel_dimensions.x) / 2);
        }
        if (camera->h > map->pixel_dimensions.y)
        {
            position->y = -((camera->h - map->pixel_dimensions.y) / 2);
        }
    }
}

void ECS::render_map(ECS::Map *m, double ts)
{
    Rect *camera = Window::get_camera();
    for (int i = 0; i < m->grid.size(); ++i)
    {
        for (int j = 0; j < m->grid[i].size(); ++j)
        {
            Tile t = m->grid[i][j].tile;
            V2 render_position = {t.position.x - camera->x, t.position.y - camera->y};
            Rect r = {t.position.x, t.position.y, 16, 16};
            if (Physics::check_collision(camera, &r))
            {
                Render::render_texture(
                    Render::Layer::WORLD_LAYER,
                    t.texture_index,
                    t.clip,
                    render_position,
                    nullptr,
                    1,
                    0);
            }
        }
    }
}

ECS::Manager::Manager(){};

void ECS::Manager::update(double ts)
{
    for (Entity &e : this->entities)
    {
        ECS::input_system(&this->map, &e, ts);
        ECS::camera_system(&e);
        ECS::render_system(&e);
    }
};