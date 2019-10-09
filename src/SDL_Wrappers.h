#ifndef SDL_Wrappers_h_
#define SDL_Wrappers_h_
#include <SDL2/SDL.h>

bool initializeSDL(int screenWidth, int screenHeight);
SDL_Renderer *getRenderer();
void quitSDL();

#endif