#ifndef CONSOLE_h_
#define CONSOLE_h_

#include "GUI.h"
#include "EventBus.h"

struct Console
{
    Console(EventBus *);
    void update(double ts);
    UIPanel console_panel;
    EventBus *event_bus;
};

#endif