#ifndef INPUTSYSTEM_h_
#define INPUTSYSTEM_h_

#include "SDLWrapper.h"
#include "EventBus.h"

struct InputSystem
{
    InputSystem(EventBus *);
    ~InputSystem();
    void collect_input_events();
    EventBus *event_bus;
    bool quit;
};

#endif