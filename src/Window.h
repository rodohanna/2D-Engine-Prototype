#ifndef WINDOW_h_
#define WINDOW_h_

#include "GameTypes.h"

namespace Window
{
Rect *get_camera();
Rect *get_gui_camera();
void set_camera(Rect);
void set_camera(Rect *);
void set_camera_position(V2);
void set_camera_position(V2*);
void set_gui_camera(Rect);
void set_gui_camera(Rect *);
void set_world_render_scale(double);
void set_gui_render_scale(double);
V2 *get_mouse_position();
V2 *get_gui_mouse_position();
double get_world_render_scale();
double get_gui_render_scale();
}; // namespace Window

#endif