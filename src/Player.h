#ifndef PLAYER_h_
#define PLAYER_h_

#include "EventBus.h"
#include "GameTypes.h"

enum PlayerActions
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    TOTAL_PLAYER_ACTIONS
};

struct Player : IInputEventSubscriber
{
    Player(EventBus *, const Rect &, const Color &);
    ~Player();
    void update(double ts);
    void handleInputEvents(const InputEvent *, size_t);
    EventBus *eventBus;
    Rect box;
    Color color;
    bool actions[TOTAL_PLAYER_ACTIONS];
};

#endif