#ifndef CONSOLE_h_
#define CONSOLE_h_

#include "GUI.h"
#include "EventBus.h"

struct Console
{
    Console(EventBus *);
    void update(double ts);
    UIPanel console_panel;
    TextInput text_input;
    EventBus *event_bus;
};

#endif