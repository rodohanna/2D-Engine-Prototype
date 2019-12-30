#ifndef GUI_h_
#define GUI_h_

#include "BuildMenu.h"
#include "BottomBar.h"

namespace GUI
{
struct GUI
{
    GUI();
    void update(double);
    void process_messages();
    BuildMenu build_menu;
    BottomBar bottom_bar;
    bool build_menu_shown;
};
}; // namespace GUI

#endif