#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include "Events.h"
#include "SDLWrapper.h"
#include <vector>

const static size_t RENDER_QUEUE_SIZE = 2048;
const static size_t INPUT_QUEUE_SIZE = 1024;
const static size_t DEBUG_QUEUE_SIZE = 1024;

struct IInputEventSubscriber
{
    virtual ~IInputEventSubscriber() = default;
    virtual void handle_input_events(const Events::InputEvent *, size_t) = 0;
};

struct IRenderEventSubscriber
{
    virtual ~IRenderEventSubscriber() = default;
    virtual void handle_render_events(const Events::RenderEvent *, size_t) = 0;
};

struct IDebugEventSubscriber
{
    virtual ~IDebugEventSubscriber() = default;
    virtual void handle_debug_events(const Events::DebugEvent *, size_t) = 0;
};
struct EventBus
{
    EventBus(SDL_Renderer *);
    ~EventBus();

    void publish_input_event(const Events::InputEvent &e);
    void subscribe_to_input_events(IInputEventSubscriber *);
    void unsubscribe_to_input_events(IInputEventSubscriber *);
    void notify_input_event_subscribers();

    void publish_render_event(const Events::RenderEvent &e);
    void subscribe_to_render_events(IRenderEventSubscriber *);
    void unsubscribe_to_render_events(IRenderEventSubscriber *);
    void notify_render_event_subscribers();

    void publish_debug_event(const Events::DebugEvent &e);
    void subscribe_to_debug_events(IDebugEventSubscriber *);
    void unsubscribe_to_debug_events(IDebugEventSubscriber *);
    void notify_debug_event_subscribers();

    void clear_input_events();
    void clear_render_events();
    void clear_debug_events();
    std::vector<IInputEventSubscriber *> input_event_subscribers;
    std::vector<IRenderEventSubscriber *> render_event_subscribers;
    std::vector<IDebugEventSubscriber *> debug_event_subscribers;
    Events::InputEvent input_queue[INPUT_QUEUE_SIZE];
    Events::RenderEvent render_queue[RENDER_QUEUE_SIZE];
    Events::DebugEvent debug_queue[DEBUG_QUEUE_SIZE];
    size_t input_queue_length, render_queue_length, debug_queue_length;
    SDL_Renderer *renderer;
};

#endif