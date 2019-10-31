#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include "GameEvent.h"
#include "GameEntity.h"
#include "GameState.h"
#include <memory>

struct Subscriber
{
    virtual ~Subscriber() = default;
    virtual void handleEvent(GameEvent *e, GameState *s) = 0;
};

void initializeEventBus(GameState *s);
void publish(std::unique_ptr<GameEvent> e);
void subscribe(GameEventType t, Subscriber *entity);
void unsubscribe(GameEventType t, Subscriber *entity);

#endif