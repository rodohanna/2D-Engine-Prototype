#include "SDLWrapper.h"
#include "Assets.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "ProcGen.h"
#include "GameTypes.h"
#include "Order.h"
#include "GUI.h"
#include "Serialize.h"
#include "Debug.h"
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#endif

struct EngineContext
{
    double time_step;
    int refresh_rate;
    bool initialized;
};

double get_seconds_elapsed(int64_t old_counter, int64_t current_counter);
EngineContext engine_init();
void load_resources();

int main(int argc, char *argv[])
{
    EngineContext context = engine_init();
    if (!context.initialized)
    {
        printf("Engine could not be initialized.\n");
        return 1;
    }
    load_resources();

    int64_t last_counter = SDL_GetPerformanceCounter();

    // debug
    Debug debugger;
    ProcGen::Rules rules = {100, 100};
    V2 dimensions = {100, 100};
    ProcGen::Return r = ProcGen::generate_map(&rules, &dimensions);
    Order::Manager order_manager = Order::Manager();
    GUI::GUI gui;

    while (Input::is_running())
    {
        Input::collect_input_events();

        // update
        gui.process_messages();
        MBus::clear_gui_messages();
        gui.update(context.time_step);

        r.entity_manager.process_messages();
        MBus::clear_ecs_messages();
        r.entity_manager.update_player(context.time_step);
        r.entity_manager.update(context.time_step);

        order_manager.process_messages(&r.entity_manager.map);
        MBus::clear_order_messages();
        order_manager.update(&r.entity_manager.map, context.time_step);

        ECS::render_map(&r.entity_manager.map, context.time_step);

        debugger.process_messages();
        MBus::clear_debug_messages();
        debugger.update(context.time_step);

        {
            // DEBUG - SERIALIZATION
            if (Input::is_input_active(Input::Q_KEY_DOWN) && Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
            {
                Serialize::save_map(&r.entity_manager.map);
            }
        }

        if (get_seconds_elapsed(last_counter, SDL_GetPerformanceCounter()) < context.time_step)
        {
            int64_t time_to_sleep = ((context.time_step - get_seconds_elapsed(last_counter, SDL_GetPerformanceCounter())) * 1000) - 1;
            if (time_to_sleep > 0)
            {
                SDL_Delay(time_to_sleep);
            }
            while (get_seconds_elapsed(last_counter, SDL_GetPerformanceCounter()) < context.time_step)
            {
                // Waiting...
            }
        }
        else
        {
            printf("Frame took %f seconds for a %f time step.\n", get_seconds_elapsed(last_counter, SDL_GetPerformanceCounter()), context.time_step);
        }
        int64_t end_counter = SDL_GetPerformanceCounter();

        Render::perform_render();

        last_counter = end_counter;
    }
    return 0;
}

double get_seconds_elapsed(int64_t old_counter, int64_t current_counter)
{
    return ((double)(current_counter - old_counter) / (double)(SDL_GetPerformanceFrequency()));
}

EngineContext engine_init()
{
    EngineContext context;
    context.initialized = true;
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
        context.initialized = false;
        return context;
    }
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    if (SDL_GetDisplayMode(0, 0, &mode) != 0)
    {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        context.refresh_rate = 60;
        context.time_step = 1.f / 60.f;
    }
    else
    {
        double ts = 1.f / 60.f;
        if (mode.refresh_rate > 0 && SDL::is_vsync())
        {
            ts = 1.f / (double)mode.refresh_rate;
        }
        context.refresh_rate = mode.refresh_rate;
        context.time_step = ts;
    }
    printf("Initializing with ts: %f\n", context.time_step);
    printf("Refresh rate: %d\n", context.refresh_rate);
    Window::set_camera({0, 0, 800, 640});
    Window::set_gui_camera({0, 0, 800, 640});
    Input::init({800, 640});
    return context;
};

void load_resources()
{
    Assets::load_assets_from_manifest(SDL::get_renderer(), "assets/asset-manifest.txt");
};