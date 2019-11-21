#ifndef GUI_h_
#define GUI_h_

#include "GameTypes.h"
#include "EventBus.h"
#include <string>
#include <vector>
#include <memory>

struct Text
{
    Text(EventBus *, size_t font_index, std::string texture_key);
    void update(double ts, size_t z_index = 1);
    void set_text(std::string);
    V2 dimensions;
    V2 position;
    Rect overflow_clip;
    EventBus *event_bus;
    size_t font_index;
    size_t texture_index;
    std::string text;
    std::string texture_key;
};

enum AnchorType
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    CENTER
};

struct Anchor
{
    AnchorType type;
    size_t offset;
};

struct UIPanel
{
    void update(double ts);
    void update_rect();
    Rect rect;
    Anchor anchor_horizontal;
    Anchor anchor_vertical;
    V2 dimensions;
    std::vector<Text> panel_text;
    size_t z_index;
    EventBus *event_bus;
};

struct TextInput : IInputEventSubscriber
{
    TextInput(EventBus *e, std::string texture_key);
    ~TextInput();
    void handle_input_events(const InputEvent *, size_t);
    void update(double ts);
    Anchor anchor_horizontal;
    Anchor anchor_vertical;
    V2 dimensions;
    double blink_interval_millis;
    double blink_interval_counter;
    size_t z_index;
    std::string text_buffer;
    EventBus *event_bus;
    std::unique_ptr<Text> text;
    bool mouse_clicked;
    bool is_active;
    bool render_cursor;
};

struct GUI
{
    GUI(EventBus *);
    void update(double ts);
    EventBus *event_bus;
    UIPanel inventory_panel;
};

#endif