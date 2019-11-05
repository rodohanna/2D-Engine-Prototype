#include "InputSystem.h"

InputSystem::InputSystem(EventBus *eB) : eventBus(eB), quit(false)
{
    eB->subscribeToInputEvents(this);
}
InputSystem::~InputSystem()
{
    eventBus->unsubscribeToInputEvents(this);
}
void InputSystem::collectInputEvents()
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
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_a:
                iE.data.keyEvent.key = A_KEY;
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_s:
                iE.data.keyEvent.key = S_KEY;
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_d:
                iE.data.keyEvent.key = D_KEY;
                eventBus->publishInputEvent(iE);
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
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_a:
                iE.data.keyEvent.key = A_KEY;
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_s:
                iE.data.keyEvent.key = S_KEY;
                eventBus->publishInputEvent(iE);
                break;
            case SDLK_d:
                iE.data.keyEvent.key = D_KEY;
                eventBus->publishInputEvent(iE);
                break;
            }
        }
    }
}

void InputSystem::handleInputEvents(const InputEvent *inputEvents, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        InputEvent e = inputEvents[i];
        if (e.type == KEY_DOWN && e.data.keyEvent.key == W_KEY)
        {
            printf("W down\n");
        }
        else if (e.type == KEY_UP && e.data.keyEvent.key == W_KEY)
        {
            printf("W up\n");
        }
    }
}