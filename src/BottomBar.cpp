#include "BottomBar.h"
#include "Window.h"
#include "MessageBus.h"

BottomBar::BottomBar()
{
    UI::Button base_button;
    base_button.rect = {0, 0, 100, 40};
    base_button.idle_color = {0x11, 0x11, 0x11, 0xF0};
    base_button.hover_color = {0x1F, 0x1F, 0x1F, 0xF0};
    base_button.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    base_button.z_index = 1;

    this->build_button.button = base_button;
    this->zone_button.button = base_button;

    this->build_button.text.font_index = 0;
    this->build_button.text.has_overflow_clip = false;
    this->build_button.text.render_layer = Render::GUI_LAYER;
    this->build_button.text.texture_key = "build_button";
    this->build_button.text.set_text("Build");

    this->zone_button.text.font_index = 0;
    this->zone_button.text.has_overflow_clip = false;
    this->zone_button.text.render_layer = Render::GUI_LAYER;
    this->zone_button.text.texture_key = "test_button";
    this->zone_button.text.set_text("Zone");
};

void BottomBar::update(double ts)
{
    if (this->build_button.button.mouse_clicked)
    {
        MBus::Message message;
        message.type = MBus::Type::TOGGLE_BUILD_MENU;
        MBus::send_gui_message(&message);
    }
    else if (this->zone_button.button.mouse_clicked)
    {
        MBus::Message message;
        message = {MBus::Type::CLOSE_BUILD_MENU};
        MBus::send_gui_message(&message);
        message = {MBus::Type::BEGIN_ZONE_PLACEMENT};
        MBus::send_order_message(&message);
    }
    Rect *camera = Window::get_gui_camera();
    int bottom_of_screen = camera->h - this->build_button.button.rect.h;
    this->build_button.button.rect.x = 0;
    this->build_button.button.rect.y = bottom_of_screen;

    this->zone_button.button.rect.x = this->build_button.button.rect.x + this->build_button.button.rect.w;
    this->zone_button.button.rect.y = bottom_of_screen;
    this->build_button.update(ts);
    this->zone_button.update(ts);
}