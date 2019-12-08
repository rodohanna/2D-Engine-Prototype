#include "Window.h"

Rect camera;
Rect gui_camera;
V2 mouse_position;
V2 gui_mouse_position;

Rect *Window::get_camera()
{
    return &camera;
}
Rect *Window::get_gui_camera()
{
    return &gui_camera;
};
void Window::set_camera(Rect rect)
{
    camera = rect;
}
void Window::set_camera(Rect *rect)
{
    camera = *rect;
}
void Window::set_gui_camera(Rect rect)
{
    gui_camera = rect;
}
void Window::set_gui_camera(Rect *rect)
{
    gui_camera = *rect;
}
V2 *Window::get_mouse_position()
{
    return &mouse_position;
}
V2 *Window::get_gui_mouse_position()
{
    return &gui_mouse_position;
}