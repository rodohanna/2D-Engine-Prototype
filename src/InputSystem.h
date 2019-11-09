#ifndef INPUTSYSTEM_h_
#define INPUTSYSTEM_h_

#include "SDLWrapper.h"
#include "EventBus.h"

struct InputSystem
{
    InputSystem(EventBus *);
    ~InputSystem();
    void collectInputEvents();
    EventBus *eventBus;
    bool quit;
};

#endif