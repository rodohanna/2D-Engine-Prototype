#ifndef MESSAGEBUS_h_
#define MESSAGEBUS_h_

namespace MBus
{
const static int MESSAGE_QUEUE_SIZE = 1024;
enum Type
{
    BEGIN_ZONE_PLACEMENT,
    END_ZONE_PLACEMENT
};
struct Message
{
    Type type;
};
struct Bus
{
    Bus();
    void send_order_message(MBus::Message *);
    void clear_messages();
    MBus::Message message_queue[MBus::MESSAGE_QUEUE_SIZE];
    int message_queue_length;
};
}; // namespace MBus

#endif