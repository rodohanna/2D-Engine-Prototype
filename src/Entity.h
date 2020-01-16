#ifndef ENTITY_h_
#define ENTITY_h_

#include "GameTypes.h"
#include "Render.h"
#include "json/picojson.h"
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
    DUMB_AI_COMPONENT,
    NUM_COMPONENT_TYPES
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
    // ** For Serialization **
    int texture_key_strings_index;
    // **
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
    // Can't store strings in unions so each
    // component has a strings array that a component
    // can stuff string in and keep an index to each one.
    std::vector<std::string> strings;
    union {
        PositionComponent p;
        RenderComponent r;
        PositionAnimateComponent p_a;
    } data;
};

struct Entity
{
    Entity();
    ~Entity();
    void add_component(ECS::Component *);
    ECS::Component *get_component(ECS::Type);
    ECS::Component *components;
    int component_flags;
    int component_length;
};

struct Tile
{
    ECS::Entity tile_entity;
    bool empty;
};

struct Cell
{
    Tile tile;
    int entity_id;
    bool has_entity;
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
    ECS::Cell **grid;
    int cell_size;
    bool mouse_data_cached;
    bool hovered_cell_cached;

private:
    void recalculate_mouse_positions();
};

void input_system(ECS::Map *, Entity *, double ts);
bool render_system(Entity *);
void camera_system(Entity *);

picojson::object jsonize_component(Type, Component *);
struct ComponentizeJsonResult
{
    Component component;
    bool success;
};
ComponentizeJsonResult componentize_json(picojson::object *);
void process_map(ECS::Map *, double ts);

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