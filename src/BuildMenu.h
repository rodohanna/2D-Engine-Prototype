#ifndef BUILDMENU_h_
#define BUILDMENU_h_

#include "UI.h"
#include "Build.h"
#include <vector>
#include <unordered_map>

struct BuildMenu
{
    BuildMenu();
    void update(double, double);
    void set_buildables(std::vector<Build::Buildable> *);
    UI::Panel panel;
    std::vector<UI::TextButton> buttons;
    std::vector<UI::TextButton> build_category_buttons;
    std::unordered_map<std::string, std::vector<Build::Buildable>> build_category_to_buildables;
    std::string active_build_category;
};

#endif