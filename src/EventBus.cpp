#include "EventBus.h"
#include "Assets.h"
#include "Debug.h"
#include <stdio.h>
#include <memory>
#include <algorithm>

EventBus::EventBus(SDL_Renderer *r) : input_queue_length(0), render_queue_length(0), debug_queue_length(0), renderer(r)
{
}

EventBus::~EventBus()
{
}

void EventBus::publish_input_event(const Events::InputEvent &e)
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
    this->input_event_subscribers.push_back(subscriber);
}

void EventBus::unsubscribe_to_input_events(IInputEventSubscriber *subscriber)
{
    auto it = std::find(this->input_event_subscribers.begin(), this->input_event_subscribers.end(), subscriber);
    if (it != this->input_event_subscribers.end())
    {
        this->input_event_subscribers.erase(it);
    }
}

void EventBus::notify_input_event_subscribers()
{
    for (auto it = this->input_event_subscribers.begin(); it != this->input_event_subscribers.end(); ++it)
    {
        (*it)->handle_input_events(this->input_queue, this->input_queue_length);
    }
}

void EventBus::publish_render_event(const Events::RenderEvent &e)
{
    if (render_queue_length == RENDER_QUEUE_SIZE - 1)
    {
        printf("Warning: Render Event Queue is full. Consider increasing Queue size from %d.\n", RENDER_QUEUE_SIZE);
        return;
    }
    render_queue[render_queue_length] = e;
    ++render_queue_length;
}

void EventBus::subscribe_to_render_events(IRenderEventSubscriber *subscriber)
{
    this->render_event_subscribers.push_back(subscriber);
}

void EventBus::unsubscribe_to_render_events(IRenderEventSubscriber *subscriber)
{
    auto it = std::find(this->render_event_subscribers.begin(), this->render_event_subscribers.end(), subscriber);
    if (it != this->render_event_subscribers.end())
    {
        this->render_event_subscribers.erase(it);
    }
}

void EventBus::notify_render_event_subscribers()
{
    for (auto it = this->render_event_subscribers.begin(); it != this->render_event_subscribers.end(); ++it)
    {
        (*it)->handle_render_events(this->render_queue, this->render_queue_length);
    }
}

void EventBus::publish_debug_event(const Events::DebugEvent &e)
{
    if (debug_queue_length == DEBUG_QUEUE_SIZE - 1)
    {
        printf("Warning: Debug Event Queue is full. Consider increasing Queue size from %d.\n", DEBUG_QUEUE_SIZE);
        return;
    }
    debug_queue[debug_queue_length] = e;
    ++debug_queue_length;
}

void EventBus::set_debug_flags()
{
    Debug::handle_debug_events(this->debug_queue, this->debug_queue_length);
}

void EventBus::clear_input_events()
{
    input_queue_length = 0;
}

void EventBus::clear_render_events()
{
    render_queue_length = 0;
}

void EventBus::clear_debug_events()
{
    debug_queue_length = 0;
}
