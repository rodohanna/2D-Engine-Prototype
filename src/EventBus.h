#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include <vector>
#include "Events.h"

const static size_t RENDER_QUEUE_SIZE = 2048;
const static size_t INPUT_QUEUE_SIZE = 1024;

struct IInputEventSubscriber
{
    virtual ~IInputEventSubscriber() = default;
    virtual void handle_input_events(const InputEvent *, size_t) = 0;
};

struct IRenderEventSubscriber
{
    virtual ~IRenderEventSubscriber() = default;
    virtual void handle_render_events(const RenderEvent *, size_t, double) = 0;
};
struct EventBus
{
    EventBus();
    ~EventBus();

    void publish_input_event(const InputEvent &e);
    void subscribe_to_input_events(IInputEventSubscriber *);
    void unsubscribe_to_input_events(IInputEventSubscriber *);
    void notify_input_event_subscribers();

    void publish_render_event(const RenderEvent &e);
    void subscribe_to_render_Events(IRenderEventSubscriber *);
    void unsubscribe_to_render_Events(IRenderEventSubscriber *);
    void notify_render_event_subscribers(double alpha);

    void clear_input_events();
    void clear_render_events();
    std::vector<IInputEventSubscriber *> inputEventSubscribers;
    std::vector<IRenderEventSubscriber *> renderEventSubscribers;
    InputEvent input_queue[INPUT_QUEUE_SIZE];
    RenderEvent render_queue[RENDER_QUEUE_SIZE];
    size_t input_queue_length, render_queue_length;
};

#endif