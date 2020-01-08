#include "BuildMenu.h"
#include "Input.h"
#include "Physics.h"
#include "Window.h"
#include "MessageBus.h"

BuildMenu::BuildMenu()
{
    this->panel.rect = {0, 0, 310, 300};
    this->panel.rect_color = {0x11, 0x11, 0x11, 0xF0};
    this->panel.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    this->panel.z_index = 1;

    UI::TextButton floor;
    floor.button.rect = {0, 0, 100, 75};
    floor.button.idle_color = {0x11, 0x11, 0x11, 0xF0};
    floor.button.hover_color = {0x1F, 0x1F, 0x1F, 0xF0};
    floor.button.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    floor.button.z_index = 1;
    floor.text.font_index = 0;
    floor.text.has_overflow_clip = false;
    floor.text.render_layer = Render::GUI_LAYER;
    floor.text.texture_key = "floor_button";
    floor.text.set_text("Floor");
    this->buttons.push_back(floor);
}

void BuildMenu::update(double ts)
{
    this->panel.update(ts);
    int curr_x = this->panel.rect.x + 5;
    int curr_y = this->panel.rect.y + 5;
    for (unsigned int i = 0; i < this->buttons.size(); ++i)
    {
        UI::TextButton *tb = &this->buttons[i];
        tb->button.rect.x = curr_x;
        tb->button.rect.y = curr_y;
        tb->update(ts);
        if (i % 3 == 0) // 3 per row
        {
            curr_y += tb->button.rect.h + 5;
            curr_x = this->panel.rect.x + 37;
        }
    }
    if (this->buttons[0].button.mouse_clicked) // build floor
    {
        printf("Build Floor\n");
        MBus::Message message;
        message.type = MBus::Type::BEGIN_FLOOR_PLACEMENT;
        MBus::send_order_message(&message);
    }
    if (Physics::check_point_in_rect(Window::get_gui_mouse_position(), &this->panel.rect))
    {
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            Input::clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
        }
    }
}