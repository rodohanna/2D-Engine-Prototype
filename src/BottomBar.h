#ifndef BOTTOMBAR_h_
#define BOTTOMBAR_h_

#include "UI.h"

struct BottomBar
{
    BottomBar();
    void update(double ts);
    UI::TextButton build_button;
    UI::TextButton test_button;
};

#endif