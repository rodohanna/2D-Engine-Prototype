#include "Order.h"
#include "Input.h"
#include "MessageBus.h"

Order::Manager::Manager(){};
void Order::Manager::update(ECS::Map *map, double ts)
{
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
            if (this->zone_manager.state == Zone::IDLE)
            {
                this->zone_manager.wait_for_zone_placement(map);
            }
            else
            {
                this->zone_manager.quit_zone_placement();
            }

            if (this->build_manager.state != Build::IDLE)
            {
                this->build_manager.quit_structure_placement();
            }
            break;
        }
        case MBus::Type::BEGIN_STRUCTURE_PLACEMENT:
        {
            printf("Handling structure placement\n");
            this->build_manager.begin_structure_placement();
            if (this->zone_manager.state != Zone::IDLE)
            {
                this->zone_manager.quit_zone_placement();
            }
            break;
        }
        case MBus::Type::BEGIN_FLOOR_PLACEMENT:
        {
            printf("Handling floor placement\n");
            this->build_manager.begin_floor_placement();
            if (this->zone_manager.state != Zone::IDLE)
            {
                this->zone_manager.quit_zone_placement();
            }
            break;
        }
        default:
        {
            // NOOP
        }
        }
    }
}