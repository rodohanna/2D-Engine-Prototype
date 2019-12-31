#ifndef BUILD_h_
#define BUILD_h_

#include "GameTypes.h"
#include "Entity.h"

namespace Build
{
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void begin_structure_placement(V2 *);
    void quit_and_save_structure_placement(ECS::Map *);
    void quit_structure_placement();

    V2 build_dimensions;
    bool placing_structure;
};
}; // namespace Build

#endif