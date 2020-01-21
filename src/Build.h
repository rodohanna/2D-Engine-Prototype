#ifndef BUILD_h_
#define BUILD_h_

#include "GameTypes.h"
#include "Entity.h"

namespace Build
{
enum BuildableType
{
    TILE,
    ENTITY
};
struct Buildable
{
    std::string build_category;
    ECS::Entity entity;
    BuildableType type;
};
enum State
{
    IDLE,
    WAITING_TO_BUILD_TILE,
    WAITING_TO_BUILD_ENTITY,
    PLACING_FLOOR
};
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void begin_entity_placement(const ECS::Entity *);
    void begin_tile_placement(const ECS::Entity *);
    void save_entity_placement(ECS::Map *);
    void save_tile_placement(ECS::Map *);
    void quit_entity_placement();
    void quit_tile_placement();

    V2 start_floor_grid_position;
    State state;
    const ECS::Entity *blueprint;
};
}; // namespace Build

#endif