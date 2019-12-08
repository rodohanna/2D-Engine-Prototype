#ifndef WINDOW_h_
#define WINDOW_h_

#include "GameTypes.h"

namespace Window
{
Rect *get_camera();
Rect *get_gui_camera();
void set_camera(Rect);
void set_camera(Rect *);
void set_gui_camera(Rect);
void set_gui_camera(Rect*);
V2 *get_mouse_position();
V2 *get_gui_mouse_position();
}; // namespace Window

#endif