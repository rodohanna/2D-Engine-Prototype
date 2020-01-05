#ifndef DEBUG_h_
#define DEBUG_h_

#include "UI.h"
#include <sstream>

struct Debug
{
    Debug();
    void update(double);
    void process_messages();
    UI::Panel debug_panel;
    UI::Text entities_rendered_text;
    UI::Text messages_in_render_queue_text;
    UI::Text tiles_rendered_text;
    UI::Text entities_processed_text;
    std::stringstream converter;
    int entities_rendered;
    int messages_in_render_queue;
    int tiles_rendered;
    int entities_processed;
};
#endif