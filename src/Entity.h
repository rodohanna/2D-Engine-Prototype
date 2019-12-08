#ifndef ENTITY_h_
#define ENTITY_h_

#include "GameTypes.h"
#include "Render.h"
#include <vector>
#include <unordered_map>

namespace ECS
{

enum Type
{
    POSITION,
    RENDER,
    CAMERA
};

struct PositionComponent
{
    V2 position;
};

struct RenderComponent
{
    Rect clip;
    Render::Layer layer;
    int texture_index;
    int scale;
    int z_index;
    bool has_clip;
};

struct Component
{
    ECS::Type type;
    union {
        PositionComponent p;
        RenderComponent r;
    } data;
};

struct Entity
{
    std::unordered_map<ECS::Type, Component> components;
    int energy;
};

void render_system(Entity *);
void camera_system(Entity *);

struct Manager
{
    void take_turns();
    void update(double);
    std::vector<Entity> entities;
};

}; // namespace ECS

#endif