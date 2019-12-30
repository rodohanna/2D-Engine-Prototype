#include "MessageBus.h"
#include <stdio.h>

MBus::Message order_message_queue[MBus::ORDER_MESSAGE_QUEUE_SIZE];
MBus::Message ecs_message_queue[MBus::ECS_MESSAGE_QUEUE_SIZE];
MBus::Message gui_message_queue[MBus::GUI_MESSAGE_QUEUE_SIZE];
int order_message_queue_length = 0;
int ecs_message_queue_length = 0;
int gui_message_queue_length = 0;

void MBus::send_order_message(MBus::Message *m)
{
    MBus::send_message(order_message_queue, m, &order_message_queue_length, MBus::ORDER_MESSAGE_QUEUE_SIZE);
};

void MBus::send_ecs_message(MBus::Message *m)
{
    MBus::send_message(ecs_message_queue, m, &ecs_message_queue_length, MBus::ECS_MESSAGE_QUEUE_SIZE);
}

void MBus::send_gui_message(MBus::Message *m)
{
    MBus::send_message(gui_message_queue, m, &gui_message_queue_length, MBus::GUI_MESSAGE_QUEUE_SIZE);
}

void MBus::send_message(MBus::Message message_queue[], MBus::Message *message, int *message_queue_length, int message_queue_size)
{
    if (*message_queue_length > message_queue_size - 1)
    {
        printf("Warning: message_queue is full, consider increasing the size from %d\n", message_queue_size);
        return;
    }
    message_queue[(*message_queue_length)++] = *message;
}

MBus::MessageQueue MBus::get_queue(MBus::QueueType q_type)
{
    MBus::MessageQueue mq = {nullptr, 0};
    if (q_type == MBus::QueueType::ORDER)
    {
        mq.queue = order_message_queue;
        mq.length = order_message_queue_length;
    }
    else if (q_type == MBus::QueueType::ECS)
    {
        mq.queue = ecs_message_queue;
        mq.length = ecs_message_queue_length;
    }
    else if (q_type == MBus::QueueType::GUI)
    {
        mq.queue = gui_message_queue;
        mq.length = gui_message_queue_length;
    }
    return mq;
}

void MBus::clear_order_messages()
{
    order_message_queue_length = 0;
}
void MBus::clear_ecs_messages()
{
    ecs_message_queue_length = 0;
}
void MBus::clear_gui_messages()
{
    gui_message_queue_length = 0;
}