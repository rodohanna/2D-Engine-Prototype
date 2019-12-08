#include "Input.h"
#include "SDLWrapper.h"
#include "Window.h"
#include <stdio.h>

const int EVENTS_SIZE = 100;
int event_queue_length = 0;
Input::Event event_queue[EVENTS_SIZE];
bool running;

void Input::init()
{
    running = true;
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        event_queue[i] = EMPTY_INPUT_EVENT;
    }
    event_queue_length = 0;
}

void update_mouse_positions()
{
    double world_render_scale = Window::get_world_render_scale();
    double gui_render_scale = Window::get_gui_render_scale();
    V2 *mouse_position = Window::get_mouse_position();
    V2 *gui_mouse_position = Window::get_gui_mouse_position();
    SDL_GetMouseState(&mouse_position->x, &mouse_position->y);
    SDL_GetMouseState(&gui_mouse_position->x, &gui_mouse_position->y);
    mouse_position->x /= world_render_scale;
    mouse_position->y /= world_render_scale;
    gui_mouse_position->x /= gui_render_scale;
    gui_mouse_position->y /= gui_render_scale;
}

void update_cameras(double w, double h)
{
    double world_render_scale = Window::get_world_render_scale();
    double gui_render_scale = Window::get_gui_render_scale();
    Rect *camera = Window::get_camera();
    Rect *gui_camera = Window::get_gui_camera();
    camera->w = w / world_render_scale;
    camera->h = h / world_render_scale;
    gui_camera->w = w / gui_render_scale;
    gui_camera->h = h / gui_render_scale;
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
                update_cameras(e.window.data1, e.window.data2);
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
    update_mouse_positions();
}

void Input::register_input(Input::Event e)
{
    if (event_queue_length >= EVENTS_SIZE - 1)
    {
        printf("ERROR: input event queue is full and event_queue are being dropped! Consider increasing event queue size.\n");
        return;
    }
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (event_queue[i] == Input::EMPTY_INPUT_EVENT)
        {
            event_queue[i] = e;
            ++event_queue_length;
            return;
        }
    }
    printf("ERROR: Couldn't find spot for input event despite length being less than size!\n");
}

bool Input::is_running()
{
    return running;
}