#ifndef GUI_h_
#define GUI_h_

#include "GameTypes.h"
#include "EventBus.h"
#include <string>
#include <vector>
#include <memory>

struct ITextInputEnterHandler
{
    virtual ~ITextInputEnterHandler() = default;
    virtual void handle_text_input_enter_pressed() = 0;
};

struct IButtonClickHandler
{
    virtual ~IButtonClickHandler() = default;
    virtual void handle_button_clicked() = 0;
};

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

enum TextAlign
{
    TEXT_ALIGN_TOP,
    TEXT_ALIGN_BOTTOM
};

struct Anchor
{
    AnchorType type;
    int offset;
};

struct UIPanel
{
    void update(double ts);
    void update_rect();
    Rect rect;
    Anchor anchor_horizontal;
    Anchor anchor_vertical;
    V2 dimensions;
    TextAlign text_align;
    std::vector<Text> panel_text;
    size_t z_index;
    EventBus *event_bus;
};

struct Button : IInputEventSubscriber
{
    Button(EventBus *e, std::string text, std::string texture_key);
    ~Button();
    void update(double ts);
    void handle_input_events(const Events::InputEvent *, size_t);
    void add_click_handler(IButtonClickHandler *);
    void remove_click_handler(IButtonClickHandler *);
    V2 dimensions;
    Anchor anchor_horizontal;
    Anchor anchor_vertical;
    size_t z_index;
    EventBus *event_bus;
    std::unique_ptr<Text> text;
    std::vector<IButtonClickHandler *> click_handlers;
    bool mouse_clicked;
};

struct TextInput : IInputEventSubscriber
{
    TextInput(EventBus *e, std::string texture_key);
    ~TextInput();
    void handle_input_events(const Events::InputEvent *, size_t);
    void update(double ts);
    void add_enter_pressed_handler(ITextInputEnterHandler *);
    void remove_enter_pressed_handler(ITextInputEnterHandler *);
    void clear();
    void get_input_focus();
    void release_input_focus();
    Anchor anchor_horizontal;
    Anchor anchor_vertical;
    V2 dimensions;
    double blink_interval_millis;
    double blink_interval_counter;
    size_t z_index;
    std::string text_buffer;
    EventBus *event_bus;
    std::unique_ptr<Text> text;
    std::vector<ITextInputEnterHandler *> enter_handlers;
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