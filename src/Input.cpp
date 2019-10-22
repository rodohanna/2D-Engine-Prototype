#include "Input.h"
#include <stdio.h>

const int EVENTS_SIZE = 100;
int eventsLength = 0;
InputEvent events[EVENTS_SIZE];

void initializeInputEvents()
{
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        events[i] = EMPTY_INPUT_EVENT;
    }
}

void registerInput(InputEvent e)
{
    if (eventsLength >= EVENTS_SIZE - 1)
    {
        printf("ERROR: input event queue is full and events are being dropped! Consider increasing event queue size.\n");
        return;
    }
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (events[i] == EMPTY_INPUT_EVENT)
        {
            events[i] = e;
            ++eventsLength;
            return;
        }
    }
    printf("ERROR: Couldn't find spot for input event despite length being less than size!\n");
}
void clearInput(InputEvent e)
{
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (events[i] == e)
        {
            events[i] = EMPTY_INPUT_EVENT;
            --eventsLength;
        }
    }
}
bool isInputActive(InputEvent e)
{
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (events[i] == e)
        {
            return true;
        }
    }
    return false;
}