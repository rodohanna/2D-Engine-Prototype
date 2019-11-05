#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include <vector>
#include "Events.h"

const static size_t RENDER_QUEUE_SIZE = 2048;
const static size_t INPUT_QUEUE_SIZE = 1024;

struct IInputEventSubscriber
{
    virtual ~IInputEventSubscriber() = default;
    virtual void handleInputEvents(const InputEvent *, size_t) = 0;
};

struct IRenderEventSubscriber
{
    virtual ~IRenderEventSubscriber() = default;
    virtual void handleRenderEvents(const RenderEvent *, size_t) = 0;
};
struct EventBus
{
    EventBus();
    ~EventBus();

    void publishInputEvent(const InputEvent &e);
    void subscribeToInputEvents(IInputEventSubscriber *);
    void unsubscribeToInputEvents(IInputEventSubscriber *);
    void notifyInputEventSubscribers();

    void publishRenderEvent(const RenderEvent &e);
    void subscribeToRenderEvents(IRenderEventSubscriber *);
    void unsubscribeToRenderEvents(IRenderEventSubscriber *);
    void notifyRenderEventSubscribers();

    void clear();
    std::vector<IInputEventSubscriber *> inputEventSubscribers;
    std::vector<IRenderEventSubscriber *> renderEventSubscribers;
    InputEvent inputQueue[INPUT_QUEUE_SIZE];
    RenderEvent renderQueue[RENDER_QUEUE_SIZE];
    size_t inputQueueLength, renderQueueLength;
};

#endif