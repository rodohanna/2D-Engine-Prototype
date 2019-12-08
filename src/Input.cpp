#include "Input.h"
#include "SDLWrapper.h"
#include <stdio.h>

const int EVENTS_SIZE = 100;
int events_length = 0;
Input::Event events[EVENTS_SIZE];
bool running;

void Input::init()
{
    running = true;
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        events[i] = EMPTY_INPUT_EVENT;
    }
    events_length = 0;
}

void Input::collect_input_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    register_input(Input::W_KEY_DOWN);
                    break;
                case SDLK_a:
                    register_input(Input::A_KEY_DOWN);
                    break;
                case SDLK_s:
                    register_input(Input::S_KEY_DOWN);
                    break;
                case SDLK_d:
                    register_input(Input::D_KEY_DOWN);
                    break;
                }
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            if (e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    register_input(Input::W_KEY_UP);
                    break;
                case SDLK_a:
                    register_input(Input::A_KEY_UP);
                    break;
                case SDLK_s:
                    register_input(Input::S_KEY_UP);
                    break;
                case SDLK_d:
                    register_input(Input::D_KEY_UP);
                    break;
                }
            }
        }
        else if (e.type == SDL_TEXTINPUT)
        {
            //
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                //
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                register_input(Input::LEFT_MOUSE_PRESSED);
            }
        }
    }
}

void Input::register_input(Input::Event e)
{
    if (events_length >= EVENTS_SIZE - 1)
    {
        printf("ERROR: input event queue is full and events are being dropped! Consider increasing event queue size.\n");
        return;
    }
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (events[i] == Input::EMPTY_INPUT_EVENT)
        {
            events[i] = e;
            ++events_length;
            return;
        }
    }
    printf("ERROR: Couldn't find spot for input event despite length being less than size!\n");
}

void Input::clear_input()
{
    events_length = 0;
}

bool Input::is_running()
{
    return running;
}