#ifndef SDLWrapper_h_
#define SDLWrapper_h_

#ifdef _WIN32

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

namespace SDL
{
bool initialize_SDL(size_t screenWidth, size_t screenHeight, bool vsync = true);
SDL_Renderer *get_renderer();
bool is_vsync();
}; // namespace SDL

#endif