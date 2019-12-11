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
        auto camera = Window::get_camera();
        V2 centered_on_player = {
            position.x + 8 - (camera->w / 2),
            position.y + 8 - (camera->h / 2)};
        V2 map_dimensions = {100 * 16,
                             100 * 16};
        if (centered_on_player.x < 0)
        {
            centered_on_player.x = 0;
        }
        if (centered_on_player.x + camera->w > map_dimensions.x)
        {
            centered_on_player.x = map_dimensions.x - camera->w;
        }
        if (centered_on_player.y < 0)
        {
            centered_on_player.y = 0;
        }
        if (centered_on_player.y + camera->h > map_dimensions.y)
        {
            centered_on_player.y = map_dimensions.y - camera->h;
        }
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
        ECS::Component position_animate;
        position_animate.type = ECS::Type::POSITION_ANIMATE;
        position_animate.data.p_a.counter = 0;
        position_animate.data.p_a.duration = .20;
        V2 start = *position;
        V2 end = *position;
        int tile_size = 16;
        int x_movement = 0;
        int y_movement = 0;
        auto w_pressed = Input::is_input_active(Input::Event::W_KEY_DOWN);
        auto s_pressed = Input::is_input_active(Input::Event::S_KEY_DOWN);
        auto a_pressed = Input::is_input_active(Input::Event::A_KEY_DOWN);
        auto d_pressed = Input::is_input_active(Input::Event::D_KEY_DOWN);
        auto existing_position_animate = e->components.find(ECS::Type::POSITION_ANIMATE);
        if (existing_position_animate != e->components.end() && (w_pressed || s_pressed || a_pressed || d_pressed))
        {
            auto where_entity_was_supposed_to_end = existing_position_animate->second.data.p_a.end;
            end = where_entity_was_supposed_to_end;
        }
        if (w_pressed)
        {
            y_movement = -tile_size;
        }
        if (s_pressed)
        {
            y_movement = tile_size;
        }
        if (a_pressed)
        {
            x_movement = -tile_size;
        }
        if (d_pressed)
        {
            x_movement = tile_size;
        }
        end.x += x_movement;
        end.y += y_movement;
        if (start.x != end.x || start.y != end.y)
        {
            position_animate.data.p_a.start = start;
            position_animate.data.p_a.end = end;
            e->components[position_animate.type] = position_animate;
        }
    }
}

void ECS::position_animate_system(Entity *e, double ts)
{
    auto position_animate_it = e->components.find(ECS::Type::POSITION_ANIMATE);
    auto position_it = e->components.find(ECS::Type::POSITION);
    if (position_animate_it != e->components.end() && position_it != e->components.end())
    {
        auto position = &position_it->second.data.p.position;
        auto start = position_animate_it->second.data.p_a.start;
        auto end = position_animate_it->second.data.p_a.end;
        auto counter = &position_animate_it->second.data.p_a.counter;
        auto duration = position_animate_it->second.data.p_a.duration;
        *counter += ts;
        if (*counter > duration)
        {
            *position = end;
            e->components.erase(position_animate_it);
        }
        else
        {
            position->x = Physics::lerp(start.x, end.x, *counter / duration);
            position->y = Physics::lerp(start.y, end.y, *counter / duration);
            if (position->x == end.x && position->y == end.y)
            {
                e->components.erase(position_animate_it);
            }
        }
    }
}

void ECS::Manager::update(double ts)
{
    for (Entity &e : this->entities)
    {
        ECS::input_system(&e);
        ECS::position_animate_system(&e, ts);
        ECS::camera_system(&e);
        ECS::render_system(&e);
    }
};