#ifndef INPUT_h_
#define INPUT_h_

enum InputEvent
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

void initializeInputEvents();
void registerInput(InputEvent e);
void clearInput(InputEvent e);
bool isInputActive(InputEvent e);

#endif