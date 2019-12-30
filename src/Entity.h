#ifndef ENTITY_h_
#define ENTITY_h_

#include "GameTypes.h"
#include "Render.h"
#include <vector>
#include <unordered_map>

namespace ECS
{

struct Tile
{
    Rect clip;
    V2 position;
    int texture_index;
};

struct Cell
{
    int entity_id;
    Tile tile;
};

struct Map
{
    std::vector<std::vector<ECS::Cell>> grid;
    V2 dimensions;
    V2 pixel_dimensions;
    int cell_size;
};

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

void input_system(ECS::Map *, Entity *, double ts);
void render_system(Entity *);
void camera_system(Entity *);

void render_map(ECS::Map *, double ts);

struct Manager
{
    Manager();
    void update_player(double);
    void update(double);
    ECS::Map map;
    std::vector<Entity> entities;
    int player_entity_index;
};

}; // namespace ECS

#endif