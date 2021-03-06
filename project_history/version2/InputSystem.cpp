#include "InputSystem.h"
#include <cstring>

InputSystem::InputSystem(EventBus *eB) : event_bus(eB), quit(false), some_gui_is_focused(false)
{
}
InputSystem::~InputSystem()
{
}
void InputSystem::collect_input_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        Events::InputEvent iE;
        if (e.type == SDL_QUIT)
        {
            this->quit = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            iE.type = Events::InputEventType::KEY_DOWN;
            if (this->some_gui_is_focused)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    iE.data.key_event.key = Events::BACKSPACE_KEY;
                    event_bus->publish_input_event(iE);
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    iE.data.key_event.key = Events::ENTER_KEY;
                    event_bus->publish_input_event(iE);
                }
                continue;
            }
            if (e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    iE.data.key_event.key = Events::W_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_a:
                    iE.data.key_event.key = Events::A_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_s:
                    iE.data.key_event.key = Events::S_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_d:
                    iE.data.key_event.key = Events::D_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                }
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            iE.type = Events::InputEventType::KEY_UP;
            if (this->some_gui_is_focused)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    iE.data.key_event.key = Events::BACKSPACE_KEY;
                    event_bus->publish_input_event(iE);
                }
                else if (e.key.keysym.sym == SDLK_KP_ENTER)
                {
                    iE.data.key_event.key = Events::ENTER_KEY;
                    event_bus->publish_input_event(iE);
                }
                continue;
            }
            if (e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    iE.data.key_event.key = Events::W_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_a:
                    iE.data.key_event.key = Events::A_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_s:
                    iE.data.key_event.key = Events::S_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                case SDLK_d:
                    iE.data.key_event.key = Events::D_KEY;
                    event_bus->publish_input_event(iE);
                    break;
                }
            }
        }
        else if (e.type == SDL_TEXTINPUT)
        {
            iE.type = Events::InputEventType::TEXT_INPUT;
            iE.data.text_input_event.is_backspace = false;
            strcpy(iE.data.text_input_event.text, e.text.text);
            event_bus->publish_input_event(iE);
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                iE.type = Events::InputEventType::WINDOW_RESIZE;
                iE.data.resize_event = {e.window.data1, e.window.data2};
                event_bus->publish_input_event(iE);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                iE.type = Events::InputEventType::MOUSE_CLICK;
                iE.data.mouse_click_event.button = Events::MouseButton::MOUSE_BUTTON_LEFT;
                event_bus->publish_input_event(iE);
            }
        }
    }
}

void InputSystem::handle_input_events(const Events::InputEvent *input_events, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        Events::InputEvent e = input_events[i];
        if (e.type == Events::InputEventType::GUI_FOCUSED)
        {
            this->some_gui_is_focused = true;
        }
        else if (e.type == Events::InputEventType::GUI_UNFOCUSED)
        {
            this->some_gui_is_focused = false;
        }
    }
}