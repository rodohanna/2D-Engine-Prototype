#ifndef INPUT_h_
#define INPUT_h_

#include "GameTypes.h"

namespace Input
{

enum Event
{
    EMPTY_INPUT_EVENT,
    LEFT_MOUSE_PRESSED,
    LEFT_MOUSE_JUST_PRESSED,
    W_KEY_DOWN,
    A_KEY_DOWN,
    S_KEY_DOWN,
    D_KEY_DOWN,
    W_KEY_UP,
    A_KEY_UP,
    S_KEY_UP,
    D_KEY_UP
};

void init(V2 window_dimensions);
void register_input(Input::Event);
void clear_input(Input::Event);
void clear_inputs();
void collect_input_events();
bool is_input_active(Input::Event);
bool is_running();
}; // namespace Input

#endif