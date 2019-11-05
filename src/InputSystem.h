#ifndef INPUTSYSTEM_h_
#define INPUTSYSTEM_h_

#include <SDL2/SDL.h>
#include "EventBus.h"

struct InputSystem : IInputEventSubscriber
{
    InputSystem(EventBus *);
    ~InputSystem();
    void collectInputEvents();
    void handleInputEvents(const InputEvent *, size_t);
    EventBus *eventBus;
    bool quit;
};

#endif