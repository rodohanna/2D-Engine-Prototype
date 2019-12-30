#include "BottomBar.h"
#include "Window.h"

BottomBar::BottomBar()
{
    UI::Button base_button;
    base_button.rect = {0, 0, 100, 40};
    base_button.idle_color = {0x11, 0x11, 0x11, 0xF0};
    base_button.hover_color = {0x1F, 0x1F, 0x1F, 0xF0};
    base_button.outline_color = {0xFF, 0xFF, 0xFF, 0xF0};
    base_button.z_index = 1;

    this->build_button.button = base_button;
    this->test_button.button = base_button;

    this->build_button.text.font_index = 0;
    this->build_button.text.has_overflow_clip = false;
    this->build_button.text.render_layer = Render::GUI_LAYER;
    this->build_button.text.texture_key = "build_button";
    this->build_button.text.set_text("Build");

    this->test_button.text.font_index = 0;
    this->test_button.text.has_overflow_clip = false;
    this->test_button.text.render_layer = Render::GUI_LAYER;
    this->test_button.text.texture_key = "test_button";
    this->test_button.text.set_text("Test");
};

void BottomBar::update(double ts)
{
    Rect *camera = Window::get_gui_camera();
    int bottom_of_screen = camera->h - this->build_button.button.rect.h;
    this->build_button.button.rect.x = 0;
    this->build_button.button.rect.y = bottom_of_screen;

    this->test_button.button.rect.x = this->build_button.button.rect.x + this->build_button.button.rect.w;
    this->test_button.button.rect.y = bottom_of_screen;
    this->build_button.update(ts);
    this->test_button.update(ts);
}