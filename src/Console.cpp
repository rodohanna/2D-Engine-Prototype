#include "Console.h"
#include "Window.h"
Console::Console(EventBus *e) : event_bus(e)
{
    // init UIPanel
    this->console_panel.event_bus = e;
    this->console_panel.dimensions = {400, 100};
    this->console_panel.anchor_horizontal = {AnchorType::LEFT, -this->console_panel.dimensions.x};
    this->console_panel.anchor_vertical = {AnchorType::TOP, 5};
    this->console_panel.z_index = 5;
    Text t = Text(e, 0, "console-text");
    t.set_text("TODO: Add Console Here");
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);

    TextInput *text_input = new TextInput(e, "console-input");
    this->text_input = std::unique_ptr<TextInput>(text_input);
    this->text_input->dimensions = {400, 50};
    this->text_input->anchor_horizontal = {AnchorType::LEFT, -this->text_input->dimensions.x};
    this->text_input->anchor_vertical = {AnchorType::TOP, 110};
    this->text_input->z_index = 5;
    this->text_input->add_enter_pressed_handler(this);

    Button *button = new Button(e, ">", "console-button");
    this->button = std::unique_ptr<Button>(button);
    this->button->anchor_horizontal = {AnchorType::LEFT, 5};
    this->button->anchor_vertical = {AnchorType::TOP, 165};
    this->button->dimensions = {50, 25};
    this->button->z_index = 5;
    this->button->add_click_handler(this);
};

Console::~Console()
{
    this->text_input->remove_enter_pressed_handler(this);
    this->button->remove_click_handler(this);
}

void Console::update(double ts)
{
    this->console_panel.update(ts);
    this->text_input->update(ts);
    this->panel_animator.update(ts, &this->console_panel.anchor_horizontal.offset);
    this->text_input_animator.update(ts, &this->text_input->anchor_horizontal.offset);
    this->button->update(ts);
};

void Console::handle_text_input_enter_pressed()
{
    printf("Text Input Buffer: %s\n", this->text_input->text_buffer.c_str());
    this->dispatch_console_command(this->text_input->text_buffer);
    this->text_input->clear();
}

void Console::dispatch_console_command(std::string command)
{
    Events::DebugEvent e = {};
    if (command == "show chunks")
    {
        e.type = Events::DebugEventType::SHOW_CHUNK_BOUNDARY;
        this->event_bus->publish_debug_event(e);
    }
    else if (command == "hide chunks")
    {
        e.type = Events::DebugEventType::HIDE_CHUNK_BOUNDARY;
        this->event_bus->publish_debug_event(e);
    }
}

void Console::handle_button_clicked()
{
    this->panel_animator.reset();
    this->panel_animator.start(.25, this->console_panel.anchor_horizontal.offset, 5);

    this->text_input_animator.reset();
    this->text_input_animator.start(.25, this->text_input->anchor_horizontal.offset, 5, .15);
}
