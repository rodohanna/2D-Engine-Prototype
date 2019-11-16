#ifndef GUI_h_
#define GUI_h_

#include "GameTypes.h"
#include "EventBus.h"
#include <string>

struct Text
{
    Text();
    Text(EventBus *, size_t font_index, std::string texture_key);
    void update(double ts);
    void set_text(std::string);
    V2 dimensions;
    V2 position;
    EventBus *event_bus;
    size_t font_index;
    size_t texture_index;
    std::string text;
    std::string texture_key;
};

struct UIPanel
{
    V2 dimensions;
};

struct GUI
{
    GUI(EventBus *);
    void update(double ts);
    EventBus *event_bus;
    UIPanel right_panel;
    Text text;
};

#endif