#include "Input.h"
#include "SDLWrapper.h"
#include "Window.h"
#include "MessageBus.h"
#include <stdio.h>

static const int EVENTS_SIZE = 100;
static int event_queue_length = 0;
static Input::Event event_queue[EVENTS_SIZE];
static bool running;

void update_cameras(double w, double h);
void update_mouse_positions();

void Input::init(V2 window_dimensions)
{
    running = true;
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        event_queue[i] = Input::EMPTY_INPUT_EVENT;
    }
    event_queue_length = 0;
    update_cameras(window_dimensions.x, window_dimensions.y);
}

void Input::collect_input_events()
{
    clear_input(Input::LEFT_MOUSE_JUST_PRESSED);
    clear_input(Input::RIGHT_MOUSE_JUST_PRESSED);
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
                {
                    register_input(Input::W_KEY_DOWN);
                    break;
                }
                case SDLK_a:
                {
                    register_input(Input::A_KEY_DOWN);
                    break;
                }
                case SDLK_s:
                {
                    register_input(Input::S_KEY_DOWN);
                    break;
                }
                case SDLK_d:
                {
                    register_input(Input::D_KEY_DOWN);
                    break;
                }
                case SDLK_q:
                {
                    register_input(Input::Q_KEY_DOWN);
                    if (Window::get_world_render_scale() > 1)
                    {
                        Window::set_world_render_scale(Window::get_world_render_scale() - .25);
                        V2 *window = Window::get_window();
                        update_cameras(window->x, window->y);
                    }

                    break;
                }
                case SDLK_e:
                {
                    if (Window::get_world_render_scale() < 4)
                    {
                        Window::set_world_render_scale(Window::get_world_render_scale() + .25);
                        V2 *window = Window::get_window();
                        update_cameras(window->x, window->y);
                    }
                    break;
                }
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
                {
                    clear_input(Input::W_KEY_DOWN);
                    break;
                }
                case SDLK_a:
                {
                    clear_input(Input::A_KEY_DOWN);
                    break;
                }
                case SDLK_s:
                {
                    clear_input(Input::S_KEY_DOWN);
                    break;
                }
                case SDLK_d:
                {
                    clear_input(Input::D_KEY_DOWN);
                    break;
                }
                case SDLK_q:
                {
                    clear_input(Input::Q_KEY_DOWN);
                    break;
                }
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
                register_input(Input::LEFT_MOUSE_JUST_PRESSED);
                register_input(Input::LEFT_MOUSE_PRESSED);
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                register_input(Input::RIGHT_MOUSE_JUST_PRESSED);
                register_input(Input::RIGHT_MOUSE_PRESSED);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                clear_input(Input::LEFT_MOUSE_PRESSED);
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                clear_input(Input::RIGHT_MOUSE_PRESSED);
            }
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            // double world_render_scale = Window::get_world_render_scale();
            // double new_render_scale = world_render_scale;
            // if (e.wheel.y > 0) // scroll up
            // {
            //     world_render_scale += .01;
            //     if (world_render_scale > 2.0)
            //     {
            //         world_render_scale = 2.0;
            //     }
            // }
            // else if (e.wheel.y < 0) // scroll down
            // {
            //     // Put code for handling "scroll down" here!
            //     world_render_scale -= .01;
            //     if (world_render_scale < 1.0)
            //     {
            //         world_render_scale = 1.0;
            //     }
            // }
            // Window::set_world_render_scale(new_render_scale);
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

void Input::clear_input(Input::Event e)
{
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        if (event_queue[i] == e)
        {
            event_queue[i] = Input::EMPTY_INPUT_EVENT;
            --event_queue_length;
        }
    }
}

void Input::clear_inputs()
{
    for (int i = 0; i < EVENTS_SIZE - 1; ++i)
    {
        event_queue[i] = Input::EMPTY_INPUT_EVENT;
    }
    event_queue_length = 0;
}

bool Input::is_input_active(Input::Event e)
{
    for (int i = 0; i < EVENTS_SIZE; ++i)
    {
        if (event_queue[i] == e)
        {
            return true;
        }
    }
    return false;
}

bool Input::is_running()
{
    return running;
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
    Window::set_window({static_cast<int>(w), static_cast<int>(h)});
    double world_render_scale = Window::get_world_render_scale();
    double gui_render_scale = Window::get_gui_render_scale();
    Rect *camera = Window::get_camera();
    Rect *gui_camera = Window::get_gui_camera();
    V2 camera_dimensions_before = {camera->w, camera->h};
    camera->w = w / world_render_scale;
    camera->h = h / world_render_scale;
    V2 camera_dimensions_after = {
        camera->w,
        camera->h};
    gui_camera->w = w / gui_render_scale;
    gui_camera->h = h / gui_render_scale;
    MBus::Message message;
    message.type = MBus::HANDLE_CAMERA_RESIZE_FOR_PLAYER;
    message.data.hcrfp.old_camera_dimensions = {camera_dimensions_before.x, camera_dimensions_before.y};
    message.data.hcrfp.new_camera_dimensions = {camera_dimensions_after.x, camera_dimensions_after.y};
    MBus::send_ecs_message(&message);
}