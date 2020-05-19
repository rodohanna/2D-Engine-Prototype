#include "BuildMenu.h"
#include "Input.h"
#include "Physics.h"
#include "Window.h"
#include "MessageBus.h"
#include <assert.h>

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
        if (build_category_button.text.dimensions.x > build_category_button.button.rect.w)
        {
            build_category_button.button.rect.w = build_category_button.text.dimensions.x + 10;
        }
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
    V2 *mouse_position = Window::get_gui_mouse_position();
    if (this->build_category_to_buildables.find(this->active_build_category) != this->build_category_to_buildables.end())
    {
        this->panel.update(ts);
        std::vector<Build::Buildable> *selected_buildables = &this->build_category_to_buildables[this->active_build_category];
        int curr_x = this->panel.rect.x + 5;
        int curr_y = this->panel.rect.y + 5;
        int i = 1;
        for (Build::Buildable &buildable : *selected_buildables)
        {
            if (buildable.entity.component_flags & ECS::RENDER_FLAG)
            {
                ECS::Component *render_component = buildable.entity.get_component(ECS::RENDER);
                assert(render_component != nullptr);
                ECS::RenderComponent render_data = render_component->data.r;
                V2 render_position = {curr_x, curr_y};
                int scale = render_data.scale * 2;
                Render::render_texture(
                    Render::GUI_LAYER,
                    render_data.texture_index,
                    render_data.clip,
                    render_position,
                    nullptr,
                    scale,
                    this->panel.z_index + 1);
                curr_x += (render_data.clip.w * scale) + 5;
                if (i % 4 == 0) // 3 per row
                {
                    curr_y += (render_data.clip.h * scale) + 5;
                    curr_x = this->panel.rect.x + 5;
                }
                ++i;

                // handle hover
                Rect render_rect = {
                    render_position.x,
                    render_position.y,
                    render_data.clip.w * scale,
                    render_data.clip.h * scale};
                if (Physics::check_point_in_rect(mouse_position, &render_rect))
                {
                    Color hover_color = {0x2E, 0xCC, 0x40, 0xFF};
                    Render::render_rectangle(
                        Render::GUI_LAYER,
                        render_rect,
                        hover_color,
                        false,
                        this->panel.z_index + 2);
                    if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
                    {
                        Input::clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
                        {
                            MBus::Message message;
                            message.type = MBus::Type::BEGIN_BUILDABLE_PLACEMENT;
                            message.data.bbp.entity = &buildable.entity;
                            message.data.bbp.type = buildable.type;
                            MBus::send_order_message(&message);
                        }
                    }
                }
            }
        }
        if (Physics::check_point_in_rect(Window::get_gui_mouse_position(), &this->panel.rect))
        {
            if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
            {
                Input::clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
            }
        }
    }
}