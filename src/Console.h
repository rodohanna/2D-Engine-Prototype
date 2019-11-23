#ifndef CONSOLE_h_
#define CONSOLE_h_

#include "GUI.h"
#include "EventBus.h"
#include "Animate.h"
#include <memory>

struct Console : ITextInputEnterHandler, IButtonClickHandler
{
    Console(EventBus *);
    ~Console();
    void update(double ts);
    void handle_text_input_enter_pressed();
    void handle_button_clicked();
    void dispatch_console_command(std::string);
    UIPanel console_panel;
    IntAnimate panel_animator;
    IntAnimate text_input_animator;
    std::unique_ptr<TextInput> text_input;
    std::unique_ptr<Button> button;
    EventBus *event_bus;
    bool console_active, should_release_text_input_focus;
};

#endif