#include "EventBus.h"
#include <stdio.h>
#include <memory>

EventBus::EventBus() : inputQueueLength(0), renderQueueLength(0)
{
}

EventBus::~EventBus()
{
}

void EventBus::publishInputEvent(const InputEvent &e)
{
    if (inputQueueLength == INPUT_QUEUE_SIZE - 1)
    {
        printf("Warning: Input Event Queue is full. Consider increasing Queue size from %zu.\n", INPUT_QUEUE_SIZE);
        return;
    }
    inputQueue[inputQueueLength] = e;
    ++inputQueueLength;
}

void EventBus::subscribeToInputEvents(IInputEventSubscriber *subscriber)
{
    this->inputEventSubscribers.push_back(subscriber);
}

void EventBus::unsubscribeToInputEvents(IInputEventSubscriber *subscriber)
{
    auto it = std::find(this->inputEventSubscribers.begin(), this->inputEventSubscribers.end(), subscriber);
    if (it != this->inputEventSubscribers.end())
    {
        this->inputEventSubscribers.erase(it);
    }
}

void EventBus::notifyInputEventSubscribers()
{
    for (auto it = this->inputEventSubscribers.begin(); it != this->inputEventSubscribers.end(); ++it)
    {
        (*it)->handleInputEvents(this->inputQueue, this->inputQueueLength);
    }
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

void EventBus::notifyRenderEventSubscribers()
{
    for (auto it = this->renderEventSubscribers.begin(); it != this->renderEventSubscribers.end(); ++it)
    {
        (*it)->handleRenderEvents(this->renderQueue, this->renderQueueLength);
    }
}

void EventBus::clear()
{
    inputQueueLength = 0;
    renderQueueLength = 0;
}
