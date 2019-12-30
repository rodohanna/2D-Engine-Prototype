#include "UI.h"
#include "Physics.h"
#include "Window.h"
#include "Input.h"
#include "SDLWrapper.h"
#include "Assets.h"

void UI::Panel::update(double ts)
{
    Render::render_rectangle(Render::Layer::GUI_LAYER, this->rect, this->rect_color, true, this->z_index);
    Render::render_rectangle(Render::Layer::GUI_LAYER, this->rect, this->outline_color, false, this->z_index);
}

void UI::Button::update(double ts)
{
    this->mouse_clicked = false;
    this->mouse_hovered = false;
    Color current_color = this->idle_color;
    if (Physics::check_point_in_rect(Window::get_gui_mouse_position(), &rect))
    {
        this->mouse_hovered = true;
        current_color = this->hover_color;
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            Input::clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
            this->mouse_clicked = true;
        }
    }
    Render::render_rectangle(Render::Layer::GUI_LAYER, this->rect, current_color, true, this->z_index);
    Render::render_rectangle(Render::Layer::GUI_LAYER, this->rect, this->outline_color, false, this->z_index);
}

void UI::Text::set_text(std::string text)
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
            SDL::get_renderer(),
            this->font_index,
            this->texture_key,
            text,
            {0xFF, 0xFF, 0xFF, 0xFF});
        this->text = text;
        this->texture_index = info.texture_index;
        this->dimensions = info.dimensions;
    }
}

void UI::Text::update(double ts)
{
    if (this->text == "")
    {
        return;
    }
    if (this->has_overflow_clip)
    {
        Render::render_texture(
            this->render_layer,
            this->texture_index,
            this->position,
            &this->overflow_clip,
            1,
            z_index);
    }
    else
    {
        Render::render_texture(
            this->render_layer,
            this->texture_index,
            this->position,
            nullptr,
            1,
            z_index);
    }
}

void UI::TextButton::update(double ts)
{
    this->button.update(ts);
    this->text.position = {
        this->button.rect.x + (this->button.rect.w - this->text.dimensions.x) / 2,
        this->button.rect.y + (this->button.rect.h - this->text.dimensions.y) / 2,
    };
    this->text.z_index = this->button.z_index + 1;
    this->text.update(ts);
}