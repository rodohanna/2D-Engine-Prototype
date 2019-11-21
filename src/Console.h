#ifndef CONSOLE_h_
#define CONSOLE_h_

#include "GUI.h"
#include "EventBus.h"
#include <memory>

struct Console : ITextInputEnterHandler
{
    Console(EventBus *);
    ~Console();
    void update(double ts);
    void handle_text_input_enter_pressed();
    UIPanel console_panel;
    std::unique_ptr<TextInput> text_input;
    EventBus *event_bus;
};

#endif