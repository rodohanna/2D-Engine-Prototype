#include "Window.h"

V2 _window;
Rect camera;
Rect gui_camera;
V2 mouse_position;
V2 gui_mouse_position;
float world_render_scale = 1.0;
float gui_render_scale = 1.0;

void Window::set_window(V2 dimensions)
{
    _window = dimensions;
};
void Window::set_window(V2 *dimensions)
{
    _window = *dimensions;
};
V2 *Window::get_window()
{
    return &_window;
}
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
void Window::set_camera_position(V2 position)
{
    camera.x = position.x;
    camera.y = position.y;
}
void Window::set_camera_position(V2 *position)
{
    camera.x = position->x;
    camera.y = position->y;
}
void Window::set_gui_camera(Rect rect)
{
    gui_camera = rect;
}
void Window::set_gui_camera(Rect *rect)
{
    gui_camera = *rect;
}
void Window::set_world_render_scale(double scale)
{
    world_render_scale = scale;
}
void Window::set_gui_render_scale(double scale)
{
    gui_render_scale = scale;
}
V2 *Window::get_mouse_position()
{
    return &mouse_position;
}
V2 *Window::get_gui_mouse_position()
{
    return &gui_mouse_position;
}
double Window::get_world_render_scale()
{
    return world_render_scale;
}
double Window::get_gui_render_scale()
{
    return gui_render_scale;
}