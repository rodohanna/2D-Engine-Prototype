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

void ECS::input_system(Entity *e, double ts)
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
        V2 map_dimensions = {100 * 16,
                             100 * 16};
        auto camera = Window::get_camera();

        if (position->x < 0)
        {
            position->x = 0;
        }
        if (position->x + camera->w > map_dimensions.x)
        {
            position->x = map_dimensions.x - camera->w;
        }
        if (position->y < 0)
        {
            position->y = 0;
        }
        if (position->y + camera->h > map_dimensions.y)
        {
            position->y = map_dimensions.y - camera->h;
        }
    }
}

ECS::Manager::Manager() : player(nullptr) {}

void ECS::Manager::update(double ts)
{
    for (Entity &e : this->entities)
    {
        ECS::input_system(&e, ts);
        ECS::camera_system(&e);
        ECS::render_system(&e);
    }
};