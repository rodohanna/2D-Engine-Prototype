#include "Console.h"
#include "Camera.h"
Console::Console(EventBus *e) : event_bus(e)
{
    // init UIPanel
    this->console_panel.event_bus = e;
    this->console_panel.dimensions = {400, 100};
    this->console_panel.anchor_horizontal = {AnchorType::LEFT, 5};
    this->console_panel.anchor_vertical = {AnchorType::TOP, 5};
    this->console_panel.z_index = 5;
    Text t = Text(e, 0, "console-text");
    t.set_text("TODO: Add Console Here");
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);
    this->console_panel.panel_text.push_back(t);
};

void Console::update(double ts)
{
    this->console_panel.update(ts);
};
