#ifndef SDLWrapper_h_
#define SDLWrapper_h_
#include <SDL2/SDL.h>

struct SDLWrapper
{
    SDLWrapper();
    ~SDLWrapper();
    bool initializeSDL(size_t screenWidth, size_t screenHeight, bool vsync = true);
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool vsync;
};

#endif