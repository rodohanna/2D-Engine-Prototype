#ifndef CONSOLE_h_
#define CONSOLE_h_

#include "GUI.h"
#include "EventBus.h"
#include <memory>

struct Console
{
    Console(EventBus *);
    void update(double ts);
    UIPanel console_panel;
    std::unique_ptr<TextInput> text_input;
    EventBus *event_bus;
};

#endif