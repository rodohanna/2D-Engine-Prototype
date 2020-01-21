#ifndef MESSAGEBUS_h_
#define MESSAGEBUS_h_

#include "GameTypes.h"
#include "Entity.h"

namespace MBus
{
const static int ORDER_MESSAGE_QUEUE_SIZE = 8192;
const static int ECS_MESSAGE_QUEUE_SIZE = 8192;
const static int GUI_MESSAGE_QUEUE_SIZE = 8192;
const static int DEBUG_MESSAGE_QUEUE_SIZE = 8192;
enum Type
{
    // ORDER
    BEGIN_ZONE_PLACEMENT,
    END_ZONE_PLACEMENT,
    BEGIN_STRUCTURE_PLACEMENT,
    BEGIN_FLOOR_PLACEMENT,
    // ECS
    CREATE_PLANT_ENTITY,
    CREATE_TILE,
    HANDLE_CAMERA_RESIZE_FOR_PLAYER,
    // GUI
    TOGGLE_BUILD_MENU,
    CLOSE_BUILD_MENU,
    OPEN_BUILD_MENU,
    // DEBUG
    ENTITIES_RENDERED,
    MESSAGES_IN_RENDER_QUEUE,
    ENTITIES_PROCESSED,
    TILES_RENDERED
};
struct HandleCameraResizeForPlayer
{
    V2 old_camera_dimensions;
    V2 new_camera_dimensions;
};
struct EntitiesRendered
{
    int num;
};
struct MessagesInRenderQueue
{
    int num;
};
struct TilesRendered
{
    int num;
};
struct EntitiesProcessed
{
    int num;
};
struct CreatePlantEntity
{
    V2 grid_position;
};
struct CreateTile
{
    V2 grid_position;
    const ECS::Entity *blueprint;
};
struct BeginStructurePlacement
{
    // preview texture????
    V2 dimensions;
};
struct BeginFloorPlacement
{
    const ECS::Entity *entity;
};
struct Message
{
    Type type;
    union {
        CreatePlantEntity cpe;
        BeginStructurePlacement bsp;
        EntitiesRendered er;
        MessagesInRenderQueue mirq;
        TilesRendered tr;
        EntitiesProcessed ep;
        CreateTile ct;
        HandleCameraResizeForPlayer hcrfp;
        BeginFloorPlacement bfp;
    } data;
};
enum QueueType
{
    ORDER,
    ECS,
    GUI,
    DEBUG
};
struct MessageQueue
{
    MBus::Message *queue;
    int length;
};
void send_order_message(MBus::Message *);
void send_ecs_message(MBus::Message *);
void send_gui_message(MBus::Message *);
void send_debug_message(MBus::Message *);
void clear_order_messages();
void clear_ecs_messages();
void clear_gui_messages();
void clear_debug_messages();
void send_message(MBus::Message[], MBus::Message *, int *message_queue_length, int message_queue_size);
MBus::MessageQueue get_queue(MBus::QueueType);
}; // namespace MBus

#endif