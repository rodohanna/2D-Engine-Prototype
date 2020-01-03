#include "EventBus.h"

#include <unordered_map>
#include <vector>

GameState *state = NULL;
std::unordered_map<GameEventType, std::vector<Subscriber *>> subscriptions;

void initializeEventBus(GameState *s)
{
    state = s;
}

void publish(std::unique_ptr<GameEvent> e)
{
    printf("Event published with type %d\n", e->eventType);
    if (subscriptions.find(e->eventType) != subscriptions.end())
    {
        auto entities = subscriptions[e->eventType];
        for (auto entity : entities)
        {
            entity->handleEvent(e.get(), state);
        }
    }
    else
    {
        printf("No subscriptions for type %d found.\n", e->eventType);
    }
}

void subscribe(GameEventType t, Subscriber *entity)
{
    printf("Subscribing entity %p to event type %d\n", entity, t);
    if (subscriptions.find(t) != subscriptions.end())
    {
        subscriptions[t].push_back(entity);
    }
    else
    {
        auto vec = std::vector<Subscriber *>();
        vec.push_back(entity);
        subscriptions[t] = vec;
    }
}

void unsubscribe(GameEventType t, Subscriber *entity)
{
    printf("Unsubscribing entity %p to event type %d\n", entity, t);
    if (subscriptions.find(t) != subscriptions.end())
    {
        auto it = std::find(subscriptions[t].begin(), subscriptions[t].end(), entity);
        if (it != subscriptions[t].end())
        {
            subscriptions[t].erase(it);
        }
    }
}