#include "UI.h"
#include "Physics.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"

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
