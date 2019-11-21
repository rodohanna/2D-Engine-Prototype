#include "Window.h"

Rect camera;
V2 mouse_position;

Rect *Window::get_camera()
{
    return &camera;
}

void Window::set_camera(Rect rect)
{
    camera = rect;
}
void Window::set_camera(Rect *rect)
{
    camera = *rect;
}
V2 *Window::get_mouse_position()
{
    return &mouse_position;
}