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

void BuildMenu::set_buildables(std::vector<Build::Buildable> *buildables)
{
    // set up menu.
    for (Build::Buildable &buildable : *buildables)
    {
        if (this->build_category_to_buildables.find(buildable.build_category) == this->build_category_to_buildables.end())
        {
            this->build_category_to_buildables[buildable.build_category] = std::vector<Build::Buildable>();
        }
        this->build_category_to_buildables[buildable.build_category].push_back(buildable);
    }
    for (auto build_category_pair : this->build_category_to_buildables)
    {
        UI::TextButton build_category_button;
        build_category_button.text.font_index = 0;
        build_category_button.text.has_overflow_clip = false;
        build_category_button.text.render_layer = Render::GUI_LAYER;
        build_category_button.text.texture_key = build_category_pair.first + "_build_menu_button";
        build_category_button.text.set_text(build_category_pair.first);
        build_category_button.button.rect = {
            0,
            0,
            125,
            40};
        build_category_button.button.idle_color = {0x11, 0x11, 0x11, 0xF0};
        build_category_button.button.hover_color = {0x1F, 0x1F, 0x1F, 0xF0};
        build_category_button.button.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
        build_category_button.button.z_index = 1;
        this->build_category_buttons.push_back(build_category_button);
    }
}

void BuildMenu::update(double ts, double bottom_y) // bottom_y is where this menu can consider the bottom is
{
    int start_x = 0;
    int start_y = bottom_y;
    this->panel.rect.y = bottom_y - 40 - this->panel.rect.h - 4;
    for (UI::TextButton &build_category_button : this->build_category_buttons)
    {
        build_category_button.button.rect.x = start_x;
        build_category_button.button.rect.y = start_y - build_category_button.button.rect.h - 2;
        start_x += build_category_button.button.rect.w + 2;
        build_category_button.update(ts);
        if (build_category_button.button.mouse_clicked)
        {
            if (this->active_build_category == build_category_button.text.text)
            {
                // toggle
                this->active_build_category = "";
            }
            else
            {
                this->active_build_category = build_category_button.text.text;
            }
        }
    }
    if (this->build_category_to_buildables.find(this->active_build_category) != this->build_category_to_buildables.end())
    {
        this->panel.update(ts);
        // TODO: RENDER BUILDABLE BUTTONS IN HERE
    }
    return;
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