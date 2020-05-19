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
#include "Physics.h"
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

    printf("Loading things\n");
    Serialize::LoadThingsResult load_things_result = Serialize::load_things("resources/data/things");
    // debug
    Debug debugger;
    ProcGen::Rules rules = {100, 100};
    V2 dimensions = {100, 100};
    ProcGen::Return r = ProcGen::generate_map(&rules, &dimensions);
    Order::Manager order_manager = Order::Manager();
    GUI::GUI gui;
    gui.build_menu.set_buildables(&load_things_result.buildables);

    printf("Loading game\n");
    Serialize::LoadMapResult load_game_result = Serialize::load_game("resources/data/save.json");
    if (load_game_result.success)
    {
        // TODO: We need to save/load the player
        r.entity_manager = load_game_result.entity_manager;
        printf("Loaded game successfully.\n");
    }
    else
    {
        printf("Yikes. Couldn't load save file\n");
    }

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

        ECS::process_map(&r.entity_manager.map, context.time_step);

        debugger.process_messages();
        MBus::clear_debug_messages();
        debugger.update(context.time_step);

        {
            // DEBUG - SERIALIZATION
            if (Input::is_input_active(Input::Q_KEY_DOWN) && Input::is_input_active(Input::LEFT_MOUSE_JUST_PRESSED))
            {
                Serialize::save_game(&r.entity_manager, "resources/data/save.json");
            }
        }

        {
            // DEBUG - Grid
            if (gui.build_menu_shown)
            {
                Rect *camera = Window::get_camera();
                Color line_color = {0xFF, 0xFF, 0xFF, 0x0F};
                for (int i = 0; i <= 100; ++i)
                {
                    V2 vert_start = {(i * 32) - camera->x, 0 - camera->y};
                    V2 vert_end = {(i * 32) - camera->x, (32 * 100) - camera->y};
                    V2 horiz_start = {0 - camera->x, (i * 32) - camera->y};
                    V2 horiz_end = {(32 * 100) - camera->x, (i * 32) - camera->y};
                    Rect vert_line_rect = {(i * 32), 0, 1, std::abs(vert_start.y - vert_end.y)};
                    Rect horiz_line_rect = {0, (i * 32), std::abs(horiz_start.x - horiz_end.x), 1};
                    if (Physics::check_collision(&vert_line_rect, camera))
                    {
                        Render::render_line(
                            Render::WORLD_LAYER,
                            &vert_start,
                            &vert_end,
                            &line_color,
                            Render::Z_Index::TILE_BASE_LAYER + 1);
                    }
                    if (Physics::check_collision(&horiz_line_rect, camera))
                    {
                        Render::render_line(
                            Render::WORLD_LAYER,
                            &horiz_start,
                            &horiz_end,
                            &line_color,
                            Render::Z_Index::TILE_BASE_LAYER + 1);
                    }
                }
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