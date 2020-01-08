#ifndef BUILD_h_
#define BUILD_h_

#include "GameTypes.h"
#include "Entity.h"

namespace Build
{
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
    void begin_floor_placement();
    void save_structure_placement(ECS::Map *);
    void save_floor_placement(ECS::Map *);
    void quit_structure_placement();
    void quit_floor_placement();

    V2 start_floor_grid_position;
    State state;
};
}; // namespace Build

#endif