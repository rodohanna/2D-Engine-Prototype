#include "Order.h"
#include "Input.h"
#include "MessageBus.h"

Order::Manager::Manager() : state(Order::State::IDLE){};
void Order::Manager::update(ECS::Map *map, double ts)
{
    switch (this->state)
    {
    case Order::State::PLACING_ZONE:
    {
        if (zone_manager.state == Zone::IDLE)
        {
            this->state = Order::State::IDLE;
        }
        break;
    }
    case Order::State::PLACING_STRUCTURE:
    {
        if (this->build_manager.state == Build::IDLE)
        {
            this->state = Order::State::IDLE;
        }
        break;
    }
    default:
    {
        // no op
    }
    }
    this->zone_manager.update(map, ts);
    this->build_manager.update(map, ts);
};

void Order::Manager::process_messages(ECS::Map *map)
{
    MBus::MessageQueue mq = MBus::get_queue(MBus::QueueType::ORDER);
    for (int i = 0; i < mq.length; ++i)
    {
        MBus::Message m = mq.queue[i];
        switch (m.type)
        {
        case MBus::Type::BEGIN_ZONE_PLACEMENT:
        {
            printf("Beginning zone placement\n");
            this->state = Order::State::PLACING_ZONE;
            this->zone_manager.wait_for_zone_placement(map);
            break;
        }
        case MBus::Type::BEGIN_STRUCTURE_PLACEMENT:
        {
            printf("Handling structure placement\n");
            this->state = Order::State::PLACING_STRUCTURE;
            this->build_manager.begin_structure_placement(&m.data.bsp.dimensions);
            break;
        }
        default:
        {
            this->state = Order::State::IDLE;
        }
        }
    }
}