#ifndef MESSAGEBUS_h_
#define MESSAGEBUS_h_

#include "GameTypes.h"

namespace MBus
{
const static int ORDER_MESSAGE_QUEUE_SIZE = 8192;
const static int ECS_MESSAGE_QUEUE_SIZE = 8192;
const static int GUI_MESSAGE_QUEUE_SIZE = 8192;
enum Type
{
    BEGIN_ZONE_PLACEMENT,
    END_ZONE_PLACEMENT,
    CREATE_PLANT_ENTITY,
    // GUI
    TOGGLE_BUILD_MENU
};
struct CreatePlantEntity
{
    V2 grid_position;
};
struct Message
{
    Type type;
    union {
        CreatePlantEntity cpe;
    } data;
};
enum QueueType
{
    ORDER,
    ECS,
    GUI
};
struct MessageQueue
{
    MBus::Message *queue;
    int length;
};
void send_order_message(MBus::Message *);
void send_ecs_message(MBus::Message *);
void send_gui_message(MBus::Message *);
void clear_order_messages();
void clear_ecs_messages();
void clear_gui_messages();
void send_message(MBus::Message[], MBus::Message *, int *message_queue_length, int message_queue_size);
MBus::MessageQueue get_queue(MBus::QueueType);
}; // namespace MBus

#endif