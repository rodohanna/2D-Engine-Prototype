#include "EventBus.h"
#include <stdio.h>
#include <memory>

EventBus::EventBus() : renderQueueLength(0)
{
}

EventBus::~EventBus()
{
}

void EventBus::publishRenderEvent(const RenderEvent &e)
{
    if (renderQueueLength == RENDER_QUEUE_SIZE - 1)
    {
        printf("Warning: Render Event Queue is full. Consider increasing Queue size from %zu.\n", RENDER_QUEUE_SIZE);
        return;
    }
    renderQueue[renderQueueLength] = e;
    ++renderQueueLength;
}

void EventBus::subscribeToRenderEvents(IRenderEventSubscriber *subscriber)
{
    this->renderEventSubscribers.push_back(subscriber);
}

void EventBus::unsubscribeToRenderEvents(IRenderEventSubscriber *subscriber)
{
    auto it = std::find(this->renderEventSubscribers.begin(), this->renderEventSubscribers.end(), subscriber);
    if (it != this->renderEventSubscribers.end())
    {
        this->renderEventSubscribers.erase(it);
    }
}

void EventBus::notify()
{
    for (auto it = this->renderEventSubscribers.begin(); it != this->renderEventSubscribers.end(); ++it)
    {
        (*it)->handleRenderEvents(this->renderQueue, this->renderQueueLength);
    }
}

void EventBus::clear()
{
    renderQueueLength = 0;
}
