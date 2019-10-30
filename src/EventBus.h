#ifndef EVENTBUS_h_
#define EVENTBUS_h_

#include "GameEvent.h"
#include "GameEntity.h"
#include "GameState.h"

void initializeEventBus(GameState *s);
void publish(GameEvent e);
void subscribe(GameEventType t, GameEntity *func);
void unsubscribe(GameEventType t, GameEntity *func);

#endif