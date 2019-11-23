#include "GUI.h"
#include "Events.h"
#include "Assets.h"
#include "Window.h"
#include "Physics.h"
#include <algorithm>

Text::Text(EventBus *e, size_t font_index, std::string texture_key) : event_bus(e), font_index(font_index), texture_key(texture_key){};

void Text::set_text(std::string text)
{
    if (text == "")
    {
        this->text = "";
        this->dimensions = {};
        return;
    }
    if (this->text != text)
    {
        auto info = Assets::create_texture_from_text(
            this->event_bus->renderer,
            this->font_index,
            this->texture_key,
            text,
            {0xFF, 0xFF, 0xFF, 0xFF});
        this->text = text;
        this->texture_index = info.texture_index;
        this->dimensions = info.dimensions;
    }
}

void Text::update(double ts, size_t z_index)
{
    if (this->text == "")
    {
        return;
    }
    this->event_bus->publish_render_event(
        Events::create_render_texture_event(
            Events::RenderLayer::GUI_LAYER,
            this->texture_index,
            this->position,
            &this->overflow_clip,
            1,
            z_index));
}

V2 get_position_from_anchors(Rect *camera, V2 &dimensions, Anchor &anchor_horizontal, Anchor &anchor_vertical)
{
    V2 anchored_position = {};
    if (anchor_horizontal.type == AnchorType::RIGHT)
    {
        anchored_position.x = camera->w - dimensions.x - anchor_horizontal.offset;
    }
    else if (anchor_horizontal.type == AnchorType::LEFT)
    {
        anchored_position.x = anchor_horizontal.offset;
    }
    else
    {
        anchored_position.x = ((camera->w - dimensions.x) / 2) + anchor_horizontal.offset;
    }

    if (anchor_vertical.type == AnchorType::TOP)
    {
        anchored_position.y = anchor_vertical.offset;
    }
    else if (anchor_vertical.type == AnchorType::BOTTOM)
    {
        anchored_position.y = camera->h - dimensions.y - anchor_vertical.offset;
    }
    else
    {
        anchored_position.y = ((camera->h - dimensions.y) / 2) + anchor_vertical.offset;
    }
    return anchored_position;
};

void UIPanel::update_rect()
{
    V2 anchored_position = get_position_from_anchors(Window::get_gui_camera(), this->dimensions, this->anchor_horizontal, this->anchor_vertical);
    this->rect = {
        anchored_position.x,
        anchored_position.y,
        this->dimensions.x,
        this->dimensions.y};
    for (Text &t : this->panel_text)
    {
        t.overflow_clip = this->rect;
    }
};

void UIPanel::update(double ts)
{
    this->update_rect();
    int last_text_height = 0;
    if (this->text_align == TextAlign::TEXT_ALIGN_TOP)
    {
        for (Text &t : this->panel_text)
        {
            t.position = {
                this->rect.x + (this->rect.w - t.dimensions.x) / 2,
                this->rect.y + last_text_height + 10};
            last_text_height += t.dimensions.y;
            t.update(ts, this->z_index + 1);
        }
    }
    else
    {
        for (int i = this->panel_text.size() - 1; i >= 0; --i)
        {
            Text *t = &this->panel_text[i];
            last_text_height += t->dimensions.y;
            t->position = {
                this->rect.x + (this->rect.w - t->dimensions.x) / 2,
                this->rect.y + this->rect.h - last_text_height - 10};
            t->update(ts, this->z_index + 1);
        }
    }
    this->event_bus->publish_render_event(
        Events::create_render_rectangle_event(Events::RenderLayer::GUI_LAYER, this->rect, {0x11, 0x11, 0x11, 0xF0}, true, this->z_index));
    this->event_bus->publish_render_event(
        Events::create_render_rectangle_event(Events::RenderLayer::GUI_LAYER, this->rect, {0xFF, 0xFF, 0xFF, 0xF0}, false, this->z_index + 1));
}

Button::Button(EventBus *e, std::string text, std::string texture_key) : event_bus(e), mouse_clicked(false)
{
    e->subscribe_to_input_events(this);
    Text *t = new Text(e, 0, texture_key);
    this->text = std::unique_ptr<Text>(t);
    this->text->set_text(text);
}

Button::~Button()
{
    this->event_bus->subscribe_to_input_events(this);
}

void Button::update(double ts)
{
    Rect *gui_camera = Window::get_gui_camera();
    V2 position = get_position_from_anchors(gui_camera, this->dimensions, this->anchor_horizontal, this->anchor_vertical);
    Rect rect = {position.x, position.y, this->dimensions.x, this->dimensions.y};
    Color button_color = {0x11, 0x11, 0x11, 0xF0};

    if (Physics::check_point_in_rect(Window::get_gui_mouse_position(), &rect)) // is hovered
    {
        button_color = {0x22, 0x22, 0x22, 0xF0};
        if (mouse_clicked)
        {
            for (IButtonClickHandler *handler : this->click_handlers)
            {
                handler->handle_button_clicked();
            }
        }
    }
    this->event_bus->publish_render_event(
        Events::create_render_rectangle_event(Events::RenderLayer::GUI_LAYER, rect, button_color, true, this->z_index));
    this->text->position = {
        position.x + (this->dimensions.x - this->text->dimensions.x) / 2,
        position.y + (this->dimensions.y - this->text->dimensions.y) / 2,
    };
    // TODO: I shouldn't have to specify an overflow clip of the entire window just to get text to render.
    this->text->overflow_clip = {0, 0, gui_camera->w, gui_camera->h};
    this->text->update(ts, this->z_index + 1);
}

void Button::handle_input_events(const Events::InputEvent *input_events, size_t count)
{
    this->mouse_clicked = false;
    for (size_t i = 0; i < count; ++i)
    {
        Events::InputEvent e = input_events[i];
        if (e.type == Events::InputEventType::MOUSE_CLICK)
        {
            this->mouse_clicked = e.data.mouse_click_event.button == Events::MouseButton::MOUSE_BUTTON_LEFT;
        }
    }
}

void Button::add_click_handler(IButtonClickHandler *handler)
{
    this->click_handlers.push_back(handler);
}

void Button::remove_click_handler(IButtonClickHandler *handler)
{
    auto it = std::find(this->click_handlers.begin(), this->click_handlers.end(), handler);
    if (it != this->click_handlers.end())
    {
        this->click_handlers.erase(it);
    }
}

TextInput::TextInput(EventBus *e, std::string texture_key) : event_bus(e)
{
    e->subscribe_to_input_events(this);
    Text *text = new Text(e, 0, texture_key);
    this->text = std::unique_ptr<Text>(text);
    this->blink_interval_millis = .5;
    this->blink_interval_counter = 0;
    this->render_cursor = true;
    this->mouse_clicked = false;
    this->is_active = false;
    this->text_buffer = "";
};

TextInput::~TextInput()
{
    this->event_bus->unsubscribe_to_input_events(this);
};

void TextInput::handle_input_events(const Events::InputEvent *input_events, size_t count)
{
    this->mouse_clicked = false;
    for (size_t i = 0; i < count; ++i)
    {
        Events::InputEvent e = input_events[i];
        if (e.type == Events::InputEventType::TEXT_INPUT && this->is_active)
        {
            this->text_buffer.append(e.data.text_input_event.text);
        }
        else if (e.type == Events::InputEventType::KEY_DOWN && this->is_active)
        {
            if (e.data.key_event.key == Events::KeyEventType::BACKSPACE_KEY)
            {

                if (this->text_buffer != "")
                {
                    this->text_buffer.pop_back();
                }
            }
            else if (e.data.key_event.key == Events::KeyEventType::ENTER_KEY)
            {
                for (ITextInputEnterHandler *handler : this->enter_handlers)
                {
                    handler->handle_text_input_enter_pressed();
                }
            }
        }
        else if (e.type == Events::InputEventType::MOUSE_CLICK)
        {
            this->mouse_clicked = e.data.mouse_click_event.button == Events::MouseButton::MOUSE_BUTTON_LEFT;
        }
    }
}

void TextInput::update(double ts)
{
    V2 anchored_position = get_position_from_anchors(Window::get_gui_camera(), this->dimensions, this->anchor_horizontal, this->anchor_vertical);
    Rect input_rect = {anchored_position.x, anchored_position.y, this->dimensions.x, this->dimensions.y};
    this->event_bus->publish_render_event(
        Events::create_render_rectangle_event(Events::RenderLayer::GUI_LAYER, input_rect, {0x11, 0x11, 0x11, 0xF0}, true, this->z_index));

    if (this->mouse_clicked)
    {
        bool was_active = this->is_active;
        this->is_active = Physics::check_point_in_rect(Window::get_gui_mouse_position(), &input_rect);
        if (was_active && !this->is_active)
        {
            this->release_input_focus();
        }
        else if (!was_active && this->is_active)
        {
            this->get_input_focus();
            this->render_cursor = false; // Ensure cursor is rendered on click.
            this->blink_interval_counter = this->blink_interval_millis;
        }
    }

    this->blink_interval_counter += ts;
    if (this->blink_interval_counter > this->blink_interval_millis)
    {
        this->blink_interval_counter = 0;
        this->render_cursor = !this->render_cursor;
    }
    int cursor_height = static_cast<int>(input_rect.h * 0.5);
    this->text->overflow_clip = input_rect;
    this->text->set_text(this->text_buffer);
    Rect cursor_rect = {anchored_position.x + this->text->dimensions.x + 5, anchored_position.y + (input_rect.h / 4), 2, cursor_height};
    this->text->position = {anchored_position.x + 5, cursor_rect.y};
    this->text->update(ts, this->z_index + 1);
    if (this->is_active && this->render_cursor)
    {
        this->event_bus->publish_render_event(
            Events::create_render_rectangle_event(Events::RenderLayer::GUI_LAYER, cursor_rect, {0xFF, 0xFF, 0xFF, 0xF0}, true, this->z_index + 1));
    }
}

void TextInput::add_enter_pressed_handler(ITextInputEnterHandler *handler)
{
    this->enter_handlers.push_back(handler);
}

void TextInput::remove_enter_pressed_handler(ITextInputEnterHandler *handler)
{
    auto it = std::find(this->enter_handlers.begin(), this->enter_handlers.end(), handler);
    if (it != this->enter_handlers.end())
    {
        this->enter_handlers.erase(it);
    }
}

void TextInput::clear()
{
    this->text_buffer = "";
    this->text->set_text("");
}

void TextInput::get_input_focus()
{
    Events::InputEvent input_event = {};
    input_event.type = Events::InputEventType::GUI_FOCUSED;
    this->event_bus->publish_input_event(input_event);
    this->is_active = true;
}
void TextInput::release_input_focus()
{
    Events::InputEvent input_event = {};
    input_event.type = Events::InputEventType::GUI_UNFOCUSED;
    this->event_bus->publish_input_event(input_event);
    this->is_active = false;
}

GUI::GUI(EventBus *e) : event_bus(e)
{
    this->inventory_panel.event_bus = e;
    this->inventory_panel.dimensions = {275, 175};
    this->inventory_panel.anchor_horizontal = {AnchorType::RIGHT, 10};
    this->inventory_panel.anchor_vertical = {AnchorType::BOTTOM, 10};
    this->inventory_panel.z_index = 4;
    this->inventory_panel.text_align = TextAlign::TEXT_ALIGN_TOP;
    Text t = Text(e, 0, "test-text");
    t.set_text("Demo UI Panel");
    this->inventory_panel.panel_text.push_back(t);
}

void GUI::update(double ts)
{
    this->inventory_panel.update(ts);
};