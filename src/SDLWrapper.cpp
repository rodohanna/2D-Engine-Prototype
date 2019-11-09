#include "SDLWrapper.h"
#include <stdio.h>

SDLWrapper::SDLWrapper() {}
SDLWrapper::~SDLWrapper() {}

bool SDLWrapper::initializeSDL(size_t screenWidth, size_t screenHeight, bool vsync)
{
    this->vsync = vsync;
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

        this->window = SDL_CreateWindow("Simulation Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (this->window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            return false;
        }
        else
        {
            Uint32 flags = SDL_RENDERER_ACCELERATED;
            if (this->vsync)
            {
                flags |= SDL_RENDERER_PRESENTVSYNC;
            }
            this->renderer = SDL_CreateRenderer(this->window, -1, flags);
            if (this->renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                return false;
            }
            else
            {
                SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

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