#ifndef ORDER_h_
#define ORDER_h_

#include "Zone.h"
#include "MessageBus.h"
#include "Entity.h"
#include "Build.h"

namespace Order
{
enum State
{
    IDLE,
    PLACING_ZONE,
    PLACING_STRUCTURE
};
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void process_messages(ECS::Map *map);
    Zone::Manager zone_manager;
    Build::Manager build_manager;
    Order::State state;
};
}; // namespace Order

#endif