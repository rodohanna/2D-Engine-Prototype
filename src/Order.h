#ifndef ORDER_h_
#define ORDER_h_

#include "Zone.h"
#include "MessageBus.h"

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
    void update(Map *, double);
    void receive_order_messages(Map *map, MBus::Message *, int length);
    Zone::Manager zone_manager;
    Order::State state;
};
}; // namespace Order

#endif