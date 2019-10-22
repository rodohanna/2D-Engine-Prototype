#ifndef INPUT_h_
#define INPUT_h_

enum InputEvent
{
    EMPTY_INPUT_EVENT,
    LEFT_MOUSE_PRESSED,
    LEFT_MOUSE_JUST_PRESSED
};

void initializeInputEvents();
void registerInput(InputEvent e);
void clearInput(InputEvent e);
bool isInputActive(InputEvent e);

#endif