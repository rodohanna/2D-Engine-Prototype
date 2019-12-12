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
    CAMERA,
    PLAYER_INPUT,
    POSITION_ANIMATE,
    DUMB_AI_COMPONENT
};

struct PositionComponent
{
    V2 position;
    V2 target_position;
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

struct PositionAnimateComponent
{
    V2 start;
    V2 end;
    double counter;
    double duration;
};

struct Component
{
    ECS::Type type;
    union {
        PositionComponent p;
        RenderComponent r;
        PositionAnimateComponent p_a;
    } data;
};

struct Entity
{
    std::unordered_map<ECS::Type, Component> components;
    int energy;
};

bool input_system(Entity *);
void render_system(Entity *);
void camera_system(Entity *);
void position_animate_system(Entity *, double ts);
void dumb_ai_system(Entity *);

struct Manager
{
    Manager();
    void take_turns();
    void update(double);
    std::vector<Entity> entities;
    Entity *player;
};

}; // namespace ECS

#endif