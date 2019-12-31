#ifndef BUILDMENU_h_
#define BUILDMENU_h_

#include "UI.h"
#include <vector>

struct BuildMenu
{
    BuildMenu();
    void update(double);
    UI::Panel panel;
    std::vector<UI::TextButton> buttons;
};

#endif