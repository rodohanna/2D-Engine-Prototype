#ifndef GAMEEVENT_h_
#define GAMEEVENT_h_

enum GameEventType
{
    CLICK
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

#endif
