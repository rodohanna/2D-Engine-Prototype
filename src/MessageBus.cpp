#include "MessageBus.h"
#include <stdio.h>

MBus::Bus::Bus() : message_queue_length(0){};

void MBus::Bus::send_order_message(MBus::Message *m)
{
    if (this->message_queue_length > MBus::MESSAGE_QUEUE_SIZE - 1)
    {
        printf("Warning: message_queue is full, consider increasing the size from %d\n", MBus::MESSAGE_QUEUE_SIZE);
        return;
    }
    this->message_queue[this->message_queue_length++] = *m;
};

void MBus::Bus::clear_messages()
{
    this->message_queue_length = 0;
}