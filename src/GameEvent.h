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
    GameEventType eventType;
};
struct ClickEvent : public GameEvent
{
    ClickEventType clickEventType;
    bool handled;
};

#endif
