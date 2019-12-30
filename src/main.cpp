#include "SDLWrapper.h"
#include "Assets.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "ProcGen.h"
#include "GameTypes.h"
#include "Order.h"
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#endif

double SDL_GetSecondsElapsed(int64_t old_counter, int64_t current_counter)
{
    return ((double)(current_counter - old_counter) / (double)(SDL_GetPerformanceFrequency()));
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL); // DEBUG
#ifdef _WIN32
    if (timeBeginPeriod(1) == TIMERR_NOCANDO)
    {
        printf("Error calling timeBeginPeriod\n");
    }
    else
    {
        printf("Successfully set timer granularity to 1ms\n");
    }
#endif
    if (!SDL::initialize_SDL(800, 640))
    {
        printf("SDL failed to initialze.\n");
        return 1;
    }
    Window::set_camera({0, 0, 800, 640});
    Window::set_gui_camera({0, 0, 800, 640});
    Input::init({800, 640});
    Assets::load_assets_from_manifest(SDL::get_renderer(), "assets/asset-manifest.txt");

    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    if (SDL_GetDisplayMode(0, 0, &mode) != 0)
    {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    double ts = 1.f / 60.f;
    if (mode.refresh_rate > 0 && SDL::is_vsync())
    {
        ts = 1.f / (double)mode.refresh_rate;
    }
    // ts = 1.f / 30.f;
    printf("Initializing with ts: %f\n", ts);
    printf("Refresh rate: %d\n", mode.refresh_rate);
    int64_t last_counter = SDL_GetPerformanceCounter();

    // debug
    ProcGen::Rules rules = {100, 100};
    V2 dimensions = {100, 100};
    ProcGen::Return r = ProcGen::generate_map(&rules, &dimensions);
    Order::Manager order_manager = Order::Manager();

    while (Input::is_running())
    {
        Input::collect_input_events();

        // update
        if (Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
        {
            MBus::Message message = {MBus::Type::BEGIN_ZONE_PLACEMENT};
            MBus::send_order_message(&message);
        }
        r.entity_manager.update_player(ts);
        order_manager.update(&r.entity_manager.map, ts);
        order_manager.process_messages(&r.entity_manager.map);
        r.entity_manager.update(ts);
        ECS::render_map(&r.entity_manager.map, ts);
        MBus::clear_messages();

        if (SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) < ts)
        {
            int64_t time_to_sleep = ((ts - SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter())) * 1000) - 1;
            if (time_to_sleep > 0)
            {
                SDL_Delay(time_to_sleep);
            }
            while (SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) < ts)
            {
                // Waiting...
            }
        }
        else
        {
            printf("Frame took %f seconds for a %f time step.\n", SDL_GetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()), ts);
        }
        int64_t end_counter = SDL_GetPerformanceCounter();

        Render::perform_render();

        last_counter = end_counter;
    }
    return 0;
}