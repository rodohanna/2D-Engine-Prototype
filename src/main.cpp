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
    if (!loadImageTexture("assets/apple.png", "apple", renderer))
    {
        printf("Unable to load texture.\n");
        return 1;
    }
    if (!loadFont("assets/m5x7.ttf", "standard_font", 64))
    {
        printf("Unable to load font.\n");
    }

    std::shared_ptr<Texture> texture = getTexture("apple");
    TTF_Font *font = getFont("standard_font");
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    std::shared_ptr<Texture> textTexture;

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
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, (800 / 2) - textTexture->mWidth / 2, (640 / 6));

        // Render sprite
        texture->render(renderer, (800 / 2) - texture->mWidth / 2, (640 / 2) - texture->mHeight / 2);

        // Draw
        SDL_RenderPresent(renderer);
        ++frameCount;
    }
}