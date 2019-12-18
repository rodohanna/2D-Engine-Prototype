#ifndef ZONE_h_
#define ZONE_h_

#include "Map.h"
#include "GameTypes.h"

// TODO: Zones are managed by some other system.
// A zone only knows meta data (which grid cells are encapsulated, zone name, other future properties (crop type?))
// Zones are essentially just metadata over a subset of grid cells.
// The Zone namespace will need to either know how to create entities and place them in the grid, or this behavior will
// be managed somewhere else and this namespace just gives tools on how to create/interact with zones. (props the latter)
namespace Zone
{
void render_zone_rect(std::vector<V2> *);
struct Manager
{
    Manager();
    void update(Map *, double);
    void begin_zone_placement(Map *);
    void quit_and_save_zone_placement(Map *);
    void quit_zone_placement(Map *);

    std::vector<std::vector<V2>> zones;
    V2 start_zone_grid_position;
    bool placing_zone;
};
}; // namespace Zone

#endif