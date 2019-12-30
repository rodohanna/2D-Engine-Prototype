#ifndef BUILDMENU_h_
#define BUILDMENU_h_

#include "UI.h"

struct BuildMenu
{
    BuildMenu();
    void update(double);
    UI::Panel panel;
};

#endif