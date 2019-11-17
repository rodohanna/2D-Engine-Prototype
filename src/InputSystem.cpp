#include "InputSystem.h"

InputSystem::InputSystem(EventBus *eB) : event_bus(eB), quit(false)
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
        InputEvent iE;
        if (e.type == SDL_QUIT)
        {
            this->quit = true;
        }
        else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            iE.type = InputEventType::KEY_DOWN;
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                iE.data.key_event.key = W_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_a:
                iE.data.key_event.key = A_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_s:
                iE.data.key_event.key = S_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_d:
                iE.data.key_event.key = D_KEY;
                event_bus->publish_input_event(iE);
                break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            iE.type = InputEventType::KEY_UP;
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                iE.data.key_event.key = W_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_a:
                iE.data.key_event.key = A_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_s:
                iE.data.key_event.key = S_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_d:
                iE.data.key_event.key = D_KEY;
                event_bus->publish_input_event(iE);
                break;
            }
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                iE.type = InputEventType::WINDOW_RESIZE;
                iE.data.resize_event = {e.window.data1, e.window.data2};
                event_bus->publish_input_event(iE);
                break;
            }
        }
    }
}