#include "BuildMenu.h"
#include "Input.h"
#include "Physics.h"
#include "Window.h"

BuildMenu::BuildMenu()
{
    this->panel.rect = {0, 0, 300, 300};
    this->panel.rect_color = {0x11, 0x11, 0x11, 0xF0};
    this->panel.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    this->panel.z_index = 1;
}

void BuildMenu::update(double ts)
{
    if (Physics::check_point_in_rect(Window::get_gui_mouse_position(), &this->panel.rect))
    {
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            Input::clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
        }
    }
    this->panel.update(ts);
}