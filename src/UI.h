#ifndef UI_h_
#define UI_h_

#include "GameTypes.h"
#include "Render.h"
#include <string>

namespace UI
{
struct Panel
{
    void update(double ts);
    Rect rect;
    Color rect_color;
    Color outline_color;
    int z_index;
};
struct Button
{
    void update(double ts);
    Rect rect;
    Color idle_color;
    Color hover_color;
    Color outline_color;
    int z_index;
    bool mouse_clicked;
    bool mouse_hovered;
};
struct Text
{
    void update(double ts);
    void set_text(std::string);
    V2 dimensions;
    V2 position;
    Rect overflow_clip;
    int font_index;
    int texture_index;
    std::string text;
    std::string texture_key;
    Render::Layer render_layer;
    int z_index;
    bool has_overflow_clip;
};
struct TextButton
{
    void update(double ts);
    Text text;
    Button button;
};
} // namespace UI

#endif