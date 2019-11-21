#ifndef INPUTSYSTEM_h_
#define INPUTSYSTEM_h_

#include "SDLWrapper.h"
#include "EventBus.h"

struct InputSystem
{
    InputSystem(EventBus *);
    ~InputSystem();
    void collect_input_events();
    void handle_input_events(const InputEvent *, size_t);
    EventBus *event_bus;
    bool quit;
    bool some_gui_is_focused;
};

#endif