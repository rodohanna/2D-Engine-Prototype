#include "GUI.h"
#include "MessageBus.h"
#include "Window.h"

GUI::GUI::GUI() : build_menu_shown(false)
{
}

void GUI::GUI::process_messages()
{
    MBus::MessageQueue mq = MBus::get_queue(MBus::GUI);
    for (int i = 0; i < mq.length; ++i)
    {
        MBus::Message message = mq.queue[i];
        if (message.type == MBus::Type::TOGGLE_BUILD_MENU)
        {
            this->build_menu_shown = !this->build_menu_shown;
        }
        else if (message.type == MBus::Type::CLOSE_BUILD_MENU)
        {
            this->build_menu_shown = false;
        }
        else if (message.type == MBus::Type::OPEN_BUILD_MENU)
        {
            this->build_menu_shown = true;
        }
    }
}

void GUI::GUI::update(double ts)
{
    // update positions
    if (this->build_menu_shown)
    {
        Rect *camera = Window::get_gui_camera();
        this->build_menu.panel.rect.x = 0;
        this->build_menu.panel.rect.y = camera->h - this->bottom_bar.build_button.button.rect.h - this->build_menu.panel.rect.h - 5;
        this->build_menu.update(ts);
    }
    this->bottom_bar.update(ts);
}