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
        if (e.type == SDL_QUIT)
        {
            this->quit = true;
        }
        else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            InputEvent iE;
            iE.type = KEY_DOWN;
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                iE.data.keyEvent.key = W_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_a:
                iE.data.keyEvent.key = A_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_s:
                iE.data.keyEvent.key = S_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_d:
                iE.data.keyEvent.key = D_KEY;
                event_bus->publish_input_event(iE);
                break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            InputEvent iE;
            iE.type = KEY_UP;
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                iE.data.keyEvent.key = W_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_a:
                iE.data.keyEvent.key = A_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_s:
                iE.data.keyEvent.key = S_KEY;
                event_bus->publish_input_event(iE);
                break;
            case SDLK_d:
                iE.data.keyEvent.key = D_KEY;
                event_bus->publish_input_event(iE);
                break;
            }
        }
    }
}