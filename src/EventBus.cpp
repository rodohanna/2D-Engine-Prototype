#include "EventBus.h"
#include <stdio.h>
#include <memory>
#include <algorithm>

EventBus::EventBus() : input_queue_length(0), render_queue_length(0)
{
}

EventBus::~EventBus()
{
}

void EventBus::publish_input_event(const InputEvent &e)
{
    if (input_queue_length == INPUT_QUEUE_SIZE - 1)
    {
        printf("Warning: Input Event Queue is full. Consider increasing Queue size from %d.\n", INPUT_QUEUE_SIZE);
        return;
    }
    input_queue[input_queue_length] = e;
    ++input_queue_length;
}

void EventBus::subscribe_to_input_events(IInputEventSubscriber *subscriber)
{
    this->inputEventSubscribers.push_back(subscriber);
}

void EventBus::unsubscribe_to_input_events(IInputEventSubscriber *subscriber)
{
    auto it = std::find(this->inputEventSubscribers.begin(), this->inputEventSubscribers.end(), subscriber);
    if (it != this->inputEventSubscribers.end())
    {
        this->inputEventSubscribers.erase(it);
    }
}

void EventBus::notify_input_event_subscribers()
{
    for (auto it = this->inputEventSubscribers.begin(); it != this->inputEventSubscribers.end(); ++it)
    {
        (*it)->handle_input_events(this->input_queue, this->input_queue_length);
    }
}

void EventBus::publish_render_event(const RenderEvent &e)
{
    if (render_queue_length == RENDER_QUEUE_SIZE - 1)
    {
        printf("Warning: Render Event Queue is full. Consider increasing Queue size from %d.\n", RENDER_QUEUE_SIZE);
        return;
    }
    render_queue[render_queue_length] = e;
    ++render_queue_length;
}

void EventBus::subscribe_to_render_Events(IRenderEventSubscriber *subscriber)
{
    this->renderEventSubscribers.push_back(subscriber);
}

void EventBus::unsubscribe_to_render_Events(IRenderEventSubscriber *subscriber)
{
    auto it = std::find(this->renderEventSubscribers.begin(), this->renderEventSubscribers.end(), subscriber);
    if (it != this->renderEventSubscribers.end())
    {
        this->renderEventSubscribers.erase(it);
    }
}

void EventBus::notify_render_event_subscribers(double alpha)
{
    for (auto it = this->renderEventSubscribers.begin(); it != this->renderEventSubscribers.end(); ++it)
    {
        (*it)->handle_render_events(this->render_queue, this->render_queue_length, alpha);
    }
}

void EventBus::clear_input_events()
{
    input_queue_length = 0;
}

void EventBus::clear_render_events()
{
    render_queue_length = 0;
}
