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
    Map();
    void update(double);
    V2 get_mouse_grid_position();
    V2 get_mouse_world_position();
    Rect get_hovered_grid_cell();
    Rect hovered_grid_cell;
    V2 dimensions;
    V2 pixel_dimensions;
    V2 mouse_grid_position;
    V2 mouse_world_position;
    std::vector<std::vector<ECS::Cell>> grid;
    int cell_size;
    bool mouse_data_cached;
    bool hovered_cell_cached;

private:
    void recalculate_mouse_positions();
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
bool render_system(Entity *);
void camera_system(Entity *);

void render_map(ECS::Map *, double ts);

struct Manager
{
    Manager();
    void update_player(double);
    void update(double);
    void process_messages();
    ECS::Map map;
    std::vector<Entity> entities;
    int player_entity_index;
};

}; // namespace ECS

#endif