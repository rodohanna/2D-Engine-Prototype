#include "Entity.h"
#include "Window.h"
#include "Input.h"
#include "Physics.h"
#include "MessageBus.h"
#include "Assets.h"
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
    for (unsigned int i = 0; i < m->grid.size(); ++i)
    {
        for (unsigned int j = 0; j < m->grid[i].size(); ++j)
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

ECS::Manager::Manager() : player_entity_index(-1){};

void ECS::Manager::update_player(double ts)
{
    if (this->player_entity_index == -1)
    {
        for (unsigned int i = 0; i < this->entities.size(); ++i)
        {
            Entity *e = &this->entities[i];
            auto input_component_it = e->components.find(ECS::Type::PLAYER_INPUT);
            auto camera_component_it = e->components.find(ECS::Type::CAMERA);
            if (input_component_it != e->components.end() && camera_component_it != e->components.end())
            {
                this->player_entity_index = i;
                break;
            }
        }
        if (this->player_entity_index == -1)
        {
            printf("WARNING: Could not find player entity\n");
            return;
        }
    }
    ECS::input_system(&this->map, &this->entities[this->player_entity_index], ts);
    ECS::camera_system(&this->entities[this->player_entity_index]);
}

void ECS::Manager::update(double ts)
{
    for (Entity &e : this->entities)
    {
        ECS::render_system(&e);
    }
};

void ECS::Manager::process_messages()
{
    MBus::MessageQueue queue = MBus::get_queue(MBus::QueueType::ECS);
    for (int i = 0; i < queue.length; ++i)
    {
        MBus::Message message = queue.queue[i];
        if (message.type == MBus::CREATE_PLANT_ENTITY)
        {
            Entity e;
            Component render_component;
            render_component.type = RENDER;
            render_component.data.r = {
                {0, 17, 16, 16},
                Render::Layer::WORLD_LAYER,
                Assets::get_texture_index("tilesheet-transparent"),
                1,
                1,
                true};
            Component position_component;
            position_component.type = POSITION;
            position_component.data.p = {message.data.cpe.grid_position.x * 16, message.data.cpe.grid_position.y * 16};
            e.components[position_component.type] = position_component;
            e.components[render_component.type] = render_component;
            this->map.grid[message.data.cpe.grid_position.x][message.data.cpe.grid_position.y].entity_id = this->entities.size();
            this->entities.push_back(e);
        }
    }
}