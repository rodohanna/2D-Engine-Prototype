#include "SDL_Wrappers.h"
#include "Assets.h"
#include "Texture.h"

int main()
{
    if (!initializeSDL(800, 640))
    {
        printf("SDL failed to initialze.\n");
        return 1;
    }
    SDL_Renderer *renderer = getRenderer();
    loadImageTexture("assets/apple.png", "apple", renderer);
    if (!loadImageTexture("assets/apple.png", "apple", renderer))
    {
        printf("Unable to load texture...\n");
        return 1;
    }

    Texture *texture = getTexture("apple");

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(renderer);
        renderTexture(texture, renderer, (800 / 2) - texture->width / 2, (640 / 2) - texture->height / 2);
        SDL_RenderPresent(renderer);
    }
}