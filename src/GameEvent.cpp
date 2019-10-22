#include "GameEvent.h"
#include <stdio.h>

const int GAME_EVENTS_SIZE = 100;
int gameEventsLength = 0;
GameEvent gameEvents[GAME_EVENTS_SIZE];

void initializeGameEvents()
{
    gameEventsLength = 0;
    for (int i = 0; i < GAME_EVENTS_SIZE - 1; ++i)
    {
        GameEvent event = {EMPTY_GAME_EVENT};
        gameEvents[i] = event;
    }
}

void registerGameEvent(GameEvent e)
{
    if (gameEventsLength >= GAME_EVENTS_SIZE - 1)
    {
        printf("ERROR: game event queue is full and events are being dropped! Consider increasing event queue size.\n");
        return;
    }
    for (int i = 0; i < GAME_EVENTS_SIZE - 1; ++i)
    {
        if (gameEvents[i].type == EMPTY_GAME_EVENT)
        {
            gameEvents[i] = e;
            ++gameEventsLength;
            return;
        }
    }
    printf("ERROR: Couldn't find spot for game event despite length being less than size!\n");
}

void clearGameEvents()
{
    initializeGameEvents();
}

GameEvent *getGameEvents()
{
    return gameEvents;
}

int getGameEventsNum()
{
    return gameEventsLength;
}