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

    UI::TextButton farmhand_house;
    farmhand_house.button.rect = {0, 0, 100, 75};
    farmhand_house.button.idle_color = {0x11, 0x11, 0x11, 0xF0};
    farmhand_house.button.hover_color = {0x1F, 0x1F, 0x1F, 0xF0};
    farmhand_house.button.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    farmhand_house.button.z_index = 1;
    farmhand_house.text.font_index = 1;
    farmhand_house.text.has_overflow_clip = false;
    farmhand_house.text.render_layer = Render::GUI_LAYER;
    farmhand_house.text.texture_key = "farmhand_house_button";
    farmhand_house.text.set_text("Farmhand House");
    this->buttons.push_back(farmhand_house);
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
    if (this->buttons[0].button.mouse_clicked) // build farm hand house
    {
        printf("Build Farmhand House\n");
        MBus::Message message;
        message.type = MBus::Type::BEGIN_STRUCTURE_PLACEMENT;
        message.data.bsp.dimensions = {3, 2};
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