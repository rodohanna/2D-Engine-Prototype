#ifndef GAMEEVENT_h_
#define GAMEEVENT_h_

#include "GameTypes.h"
#include <cstdio>

enum GameEventType
{
    CLICK,
    TILL_SOIL
};

enum ClickEventType
{
    LEFT_MOUSE,
    RIGHT_MOUSE
};

struct GameEvent
{
    virtual ~GameEvent() = default;
    GameEventType eventType;
};
struct ClickEvent : GameEvent
{
    ClickEventType clickEventType;
    bool handled;
};
struct TillSoilEvent : GameEvent
{
    TillSoilEvent(){};
    ~TillSoilEvent(){};
    V2 coords;
};

#endif
