#include "Entity.h"
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
        Render::render_texture(
            render_component.layer,
            render_component.texture_index,
            render_component.clip,
            position_component.position,
            nullptr,
            render_component.scale,
            render_component.z_index);
    }
}

void ECS::Manager::update(double)
{
    for (Entity &e : this->entities)
    {
        ECS::render_system(&e);
    }
};