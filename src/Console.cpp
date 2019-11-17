#include "Console.h"
#include "Camera.h"
Console::Console(EventBus *e) : event_bus(e)
{
    // init UIPanel
    this->console_panel.dimensions = {400, 100};
    this->console_panel.anchor_horizontal = {AnchorType::LEFT, 0};
    this->console_panel.anchor_vertical = {AnchorType::TOP, 5};
    Text t = Text(e, 0, "console-text");
    t.set_text("Yeet");
    this->console_panel.panel_text.push_back(t);
};

void Console::update(double ts)
{
    this->console_panel.update_rect();
    this->console_panel.update(ts);
};
