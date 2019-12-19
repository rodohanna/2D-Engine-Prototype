#ifndef ORDER_h_
#define ORDER_h_

#include "Zone.h"
#include "MessageBus.h"
#include "Entity.h"

namespace Order
{
enum State
{
    IDLE,
    PLACING_ZONE
};
struct Manager
{
    Manager();
    void update(ECS::Map *, double);
    void process_messages(ECS::Map *map);
    Zone::Manager zone_manager;
    Order::State state;
};
}; // namespace Order

#endif