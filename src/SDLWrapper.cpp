#include "SDLWrapper.h"
#include <stdio.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static bool vsync;

bool SDL::initialize_SDL(size_t screenWidth, size_t screenHeight, bool _vsync)
{
    vsync = _vsync;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        window = SDL_CreateWindow("Simulation Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            return false;
        }
        else
        {
            Uint32 flags = SDL_RENDERER_ACCELERATED;
            if (vsync)
            {
                flags |= SDL_RENDERER_PRESENTVSYNC;
            }
            renderer = SDL_CreateRenderer(window, -1, flags);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                return false;
            }
            else
            {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    return false;
                }

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    return false;
                }
            }
        }
    }

    return true;
}

SDL_Renderer *SDL::get_renderer()
{
    return renderer;
}

bool SDL::is_vsync()
{
    return vsync;
}