#include "Console.h"
#include "Window.h"
Console::Console(EventBus *e) : event_bus(e), console_active(false), should_release_text_input_focus(false), should_request_text_input_focus(false)
{
    this->console_panel.event_bus = e;
    this->console_panel.dimensions = {400, 100};
    this->console_panel.anchor_horizontal = {AnchorType::LEFT, -this->console_panel.dimensions.x};
    this->console_panel.anchor_vertical = {AnchorType::TOP, 5};
    this->console_panel.z_index = 5;
    this->console_panel.text_align = TextAlign::TEXT_ALIGN_BOTTOM;

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
    if (this->should_release_text_input_focus)
    {
        this->should_release_text_input_focus = false;
        this->text_input->release_input_focus();
    }
    else if (this->should_request_text_input_focus)
    {
        this->should_request_text_input_focus = false;
        this->text_input->get_input_focus();
    }
};

void Console::handle_text_input_enter_pressed()
{
    printf("Text Input Buffer: %s\n", this->text_input->text_buffer.c_str());
    this->dispatch_console_command(this->text_input->text_buffer);
    this->text_input->clear();
    this->should_release_text_input_focus = true;
}

void Console::dispatch_console_command(std::string command)
{
    Events::DebugEvent e = {};
    Text t = Text(this->event_bus, 0, command);
    t.set_text(command);
    this->console_panel.panel_text.push_back(t);
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
    else if (command == "show grid")
    {
        e.type = Events::DebugEventType::SHOW_TILE_GRID;
        this->event_bus->publish_debug_event(e);
    }
    else if (command == "hide grid")
    {
        e.type = Events::DebugEventType::HIDE_TILE_GRID;
        this->event_bus->publish_debug_event(e);
    }
}

void Console::handle_button_clicked()
{
    if (this->console_active)
    {
        this->console_active = false;
        this->button->text->set_text(">");

        this->panel_animator.reset();
        this->panel_animator.start(.25, this->console_panel.anchor_horizontal.offset, -this->console_panel.dimensions.x, .15);

        this->text_input_animator.reset();
        this->text_input_animator.start(.25, this->text_input->anchor_horizontal.offset, -this->text_input->dimensions.x);
    }
    else
    {
        this->console_active = true;
        this->should_request_text_input_focus = true;
        this->button->text->set_text("<");

        this->panel_animator.reset();
        this->panel_animator.start(.25, this->console_panel.anchor_horizontal.offset, 5);

        this->text_input_animator.reset();
        this->text_input_animator.start(.25, this->text_input->anchor_horizontal.offset, 5, .15);
    }
}
