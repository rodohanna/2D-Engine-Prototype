#include "Order.h"
#include "Input.h"
#include "MessageBus.h"
#include <assert.h>

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
                this->build_manager.quit_entity_placement();
            }
            break;
        }
        case MBus::Type::BEGIN_BUILDABLE_PLACEMENT:
        {
            printf("Handling buildable placement\n");
            assert(m.data.bbp.entity != nullptr);
            if (m.data.bbp.type == Build::BuildableType::TILE)
            {
                this->build_manager.begin_tile_placement(m.data.bbp.entity);
            }
            else
            {
                this->build_manager.begin_entity_placement(m.data.bbp.entity);
            }
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