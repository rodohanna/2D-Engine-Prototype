#ifndef BUILD_h_
#define BUILD_h_

#include "GameTypes.h"
#include "Entity.h"

namespace Build
{
struct Buildable
{
    std::string build_category;
    ECS::Entity entity;
};
enum State
{
    IDLE,
    WAITING_TO_BUILD_FLOOR,
    WAITING_TO_PLACE_STRUCTURE,
    PLACING_FLOOR
};
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void begin_structure_placement();
    void begin_floor_placement(const ECS::Entity *);
    void save_structure_placement(ECS::Map *);
    void save_floor_placement(ECS::Map *);
    void quit_structure_placement();
    void quit_floor_placement();

    V2 start_floor_grid_position;
    State state;
    const ECS::Entity *blueprint;
};
}; // namespace Build

#endif