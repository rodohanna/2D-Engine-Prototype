#include <sstream>
#include "SDL_Wrappers.h"
#include "Assets.h"
#include "Texture.h"
#include "Timer.h"
#include "TileSet.h"
#include "TileMap.h"
#include "Player.h"

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
    if (!loadImageTexture("assets/tiles.png", "tiles", renderer))
    {
        printf("Unable to load tiles.\n");
        return 1;
    }
    if (!loadImageTexture("assets/character.png", "player", renderer))
    {
        printf("Unable to load player.\n");
        return 1;
    }
    if (!loadFont("assets/m5x7.ttf", "standard_font", 32))
    {
        printf("Unable to load font.\n");
    }

    // Texture *texture = getTexture("apple");
    TTF_Font *font = getFont("standard_font");
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    std::unique_ptr<Texture> textTexture;

    bool quit = false;
    SDL_Event e;
    Timer fpsTimer = makeTimer();
    Timer fpsCapTimer = makeTimer();
    std::stringstream fpsSStream;
    Uint32 frameCount = 0;

    // create tile set
    TileSet tileSet("tiles", 80, 80, 12);
    TileMap tileMap(tileSet, "assets/test-map.map");

    // create player
    Player player(getTexture("player"));

    // create camera
    SDL_Rect camera = {0, 0, 800, 640};

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
            player.handleInput(e);
        }
        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(renderer);

        // update
        player.update(tileMap.mWidth, tileMap.mHeight);
        player.adjustCamera(camera, 800, 640, tileMap.mWidth, tileMap.mHeight);

        // Render fps
        tileMap.render(renderer, camera);
        player.render(renderer, camera);
        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Avg FPS: " << frameCount / (timerGetTicks(fpsTimer) / 1000.f);
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, 800 - textTexture->mWidth, 0);

        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Player (X: " << player.mBox.x << ", Y: " << player.mBox.y << ")";
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, 800 - textTexture->mWidth, textTexture->mHeight);

        // Render sprite
        // texture->render(renderer, (800 / 2) - texture->mWidth / 2, (640 / 2) - texture->mHeight / 2);
        // tileSet.render(renderer, (800 / 2) - tileSet.mWidth / 2, (640 / 2) - tileSet.mHeight / 2);

        // Draw
        SDL_RenderPresent(renderer);
        ++frameCount;
    }
}