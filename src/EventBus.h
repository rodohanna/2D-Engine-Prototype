#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include <vector>
#include "Events.h"

const static size_t RENDER_QUEUE_SIZE = 2048;

struct IRenderEventSubscriber
{
    virtual ~IRenderEventSubscriber() = default;
    virtual void handleRenderEvents(const RenderEvent *, size_t) = 0;
};
struct EventBus
{
    EventBus();
    ~EventBus();
    void publishRenderEvent(const RenderEvent &e);
    void subscribeToRenderEvents(IRenderEventSubscriber *);
    void unsubscribeToRenderEvents(IRenderEventSubscriber *);
    void notify();
    void clear();
    std::vector<IRenderEventSubscriber *> renderEventSubscribers;
    RenderEvent renderQueue[RENDER_QUEUE_SIZE];
    size_t renderQueueLength;
};

#endif