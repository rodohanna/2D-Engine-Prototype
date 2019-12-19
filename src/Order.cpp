#include "Order.h"
#include "Input.h"

Order::Manager::Manager() : state(Order::State::IDLE){};
void Order::Manager::update(Map *map, double ts)
{
    switch (this->state)
    {
    case Order::State::PLACING_ZONE:
    {
        if (!Input::is_input_active(Input::LEFT_MOUSE_PRESSED))
        {
            zone_manager.quit_and_save_zone_placement(map);
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
};

void Order::Manager::receive_order_messages(Map *map, MBus::Message *messages, int length)
{
    for (int i = 0; i < length; ++i)
    {
        MBus::Message m = messages[i];
        switch (m.type)
        {
        case MBus::Type::BEGIN_ZONE_PLACEMENT:
        {
            this->state = Order::State::PLACING_ZONE;
            this->zone_manager.begin_zone_placement(map);
            break;
        }
        default:
        {
            this->state = Order::State::IDLE;
        }
        }
    }
}