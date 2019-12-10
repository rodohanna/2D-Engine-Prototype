#include "Entity.h"
#include "Window.h"
#include "Input.h"
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
        auto camera = Window::get_camera();
        V2 centered_on_player = {
            position.x + 8 - (camera->w / 2),
            position.y + 8 - (camera->h / 2)};
        Window::set_camera_position(centered_on_player);
    }
}

void ECS::input_system(Entity *e)
{
    auto player_input_it = e->components.find(ECS::Type::PLAYER_INPUT);
    auto position_it = e->components.find(ECS::Type::POSITION);
    if (player_input_it != e->components.end() && position_it != e->components.end())
    {
        auto position = &position_it->second.data.p.position;
        if (Input::is_input_active(Input::Event::W_KEY_DOWN))
        {
            position->y -= 1;
        }
        if (Input::is_input_active(Input::Event::S_KEY_DOWN))
        {
            position->y += 1;
        }
        if (Input::is_input_active(Input::Event::A_KEY_DOWN))
        {
            position->x -= 1;
        }
        if (Input::is_input_active(Input::Event::D_KEY_DOWN))
        {
            position->x += 1;
        }
    }
}

void ECS::Manager::update(double)
{
    for (Entity &e : this->entities)
    {
        ECS::input_system(&e);
        ECS::camera_system(&e);
        ECS::render_system(&e);
    }
};