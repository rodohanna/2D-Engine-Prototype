#include <sstream>
#include "SDL_Wrappers.h"
#include "Assets.h"
#include "Texture.h"
#include "Timer.h"

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
        printf("Unable to load texture.\n");
        return 1;
    }
    if (!loadFont("assets/m5x7.ttf", "standard_font", 64))
    {
        printf("Unable to load font.\n");
    }

    Texture *texture = getTexture("apple");
    TTF_Font *font = getFont("standard_font");
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    Texture *textTexture = NULL;

    bool quit = false;
    SDL_Event e;
    Timer fpsTimer = makeTimer();
    Timer fpsCapTimer = makeTimer();
    std::stringstream fpsSStream;
    Uint32 frameCount = 0;

    timerStart(fpsTimer);
    while (!quit)
    {
        timerStart(fpsCapTimer);
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(renderer);

        // Render fps
        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Avg FPS: " << frameCount / (timerGetTicks(fpsTimer) / 1000.f);
        freeTexture(textTexture);
        textTexture = createTextureFromText(fpsSStream.str(), color, font, renderer);
        renderTexture(textTexture, renderer, (800 / 2) - textTexture->width / 2, (640 / 6));

        // Render sprite
        renderTexture(texture, renderer, (800 / 2) - texture->width / 2, (640 / 2) - texture->height / 2);

        // Draw
        SDL_RenderPresent(renderer);
        ++frameCount;
    }
}