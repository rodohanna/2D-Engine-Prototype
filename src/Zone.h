#ifndef ZONE_h_
#define ZONE_h_

#include "GameTypes.h"
#include "Entity.h"

// TODO: Zones are managed by some other system.
// A zone only knows meta data (which grid cells are encapsulated, zone name, other future properties (crop type?))
// Zones are essentially just metadata over a subset of grid cells.
// The Zone namespace will need to either know how to create entities and place them in the grid, or this behavior will
// be managed somewhere else and this namespace just gives tools on how to create/interact with zones. (props the latter)
namespace Zone
{
enum State
{
    IDLE,
    WAITING_TO_PLACE_ZONE,
    PLACING_ZONE
};
void render_zone_rect(std::vector<V2> *);
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void wait_for_zone_placement(ECS::Map *);
    void quit_and_save_zone_placement(ECS::Map *);
    void quit_zone_placement(ECS::Map *);

    std::vector<std::vector<V2>> zones;
    V2 start_zone_grid_position;
    State state;
};
}; // namespace Zone

#endif