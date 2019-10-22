#ifndef GAMEEVENT_h_
#define GAMEEVENT_h_

enum GameEventType
{
    EMPTY_GAME_EVENT,
    TILE_CLICKED
};

typedef struct
{
    GameEventType type;
    int tileX, tileY;
    void *tile;
} TileClickedEvent;

typedef union GameEvent {
    GameEventType type;
    TileClickedEvent tileClickedEvent;
} GameEvent;

void initializeGameEvents();
void registerGameEvent(GameEvent e);
void clearGameEvents();
GameEvent *getGameEvents();
int getGameEventsNum();

#endif
