#include "Console.h"
#include "Window.h"
Console::Console(EventBus *e) : event_bus(e)
{
    // init UIPanel
    this->console_panel.event_bus = e;
    this->console_panel.dimensions = {400, 100};
    this->console_panel.anchor_horizontal = {AnchorType::CENTER, 0};
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
    this->text_input->anchor_horizontal = {AnchorType::CENTER, 0};
    this->text_input->anchor_vertical = {AnchorType::TOP, 110};
    this->text_input->z_index = 5;
    this->text_input->add_enter_pressed_handler(this);
};

Console::~Console()
{
    this->text_input->remove_enter_pressed_handler(this);
}

void Console::update(double ts)
{
    this->console_panel.update(ts);
    this->text_input->update(ts);
};

void Console::handle_text_input_enter_pressed()
{
    printf("Text Input Buffer: %s\n", this->text_input->text_buffer.c_str());
    this->text_input->clear();
}
