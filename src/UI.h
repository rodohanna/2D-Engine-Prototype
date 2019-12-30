#ifndef UI_h_
#define UI_h_

#include "GameTypes.h"

namespace UI
{
struct Button
{
    void update(double ts);
    Rect rect;
    Color idle_color;
    Color hover_color;
    Color outline_color;
    size_t z_index;
    bool mouse_clicked;
    bool mouse_hovered;
};
} // namespace UI

#endif