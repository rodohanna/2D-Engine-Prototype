#ifndef WINDOW_h_
#define WINDOW_h_

#include "GameTypes.h"

namespace Window
{
Rect *get_camera();
void set_camera(Rect);
void set_camera(Rect *);
V2 *get_mouse_position();
}; // namespace Window

#endif