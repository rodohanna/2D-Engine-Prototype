#include <sstream>
#include <algorithm>
#include "SDL_Wrappers.h"
#include "Assets.h"
#include "Texture.h"
#include "Timer.h"
#include "TileSet.h"
#include "TileMap.h"
#include "Player.h"
#include "Input.h"
#include "GameEvent.h"

const int LEVEL_WIDTH = 800;
const int LEVEL_HEIGHT = 640;

int main()
{
    if (!initializeSDL(LEVEL_WIDTH, LEVEL_HEIGHT))
    {
        printf("SDL failed to initialze.\n");
        return 1;
    }
    SDL_Renderer *renderer = getRenderer();
    SDL_Window *window = getWindow();
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
    if (!loadImageTexture("assets/grass-tiles.png", "grass-tiles", renderer))
    {
        printf("Unable to load grass-tiles.\n");
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

    initializeInputEvents();

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
    // TileSet tileSet("tiles", 80, 80, 12);
    // TileMap tileMap(tileSet, "assets/test-map.map");

    TileSet tileSet("grass-tiles", 32, 32, 5);
    TileMap tileMap(tileSet, "assets/grass-map.map", 2);

    // create player
    Player player(getTexture("player"));

    // create camera
    SDL_Rect camera = {0, 0, LEVEL_WIDTH, LEVEL_HEIGHT};
    SDL_RenderSetLogicalSize(renderer, camera.w, camera.h);
    SDL_SetWindowMinimumSize(window, camera.w, camera.h);
    int mouseX = 0, mouseY = 0;
    timerStart(fpsTimer);
    while (!quit)
    {
        timerStart(fpsCapTimer);
        clearInput(LEFT_MOUSE_JUST_PRESSED);
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                mouseX = e.button.x;
                mouseY = e.button.y;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    registerInput(LEFT_MOUSE_PRESSED);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    registerInput(LEFT_MOUSE_JUST_PRESSED);
                    clearInput(LEFT_MOUSE_PRESSED);
                }
            }
            player.handleInput(e);
        }
        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(renderer);

        // update
        int numberOfGameEvents = getGameEventsNum();
        GameEvent *gameEvents = getGameEvents();
        for (int i = 0; i < numberOfGameEvents; ++i)
        {
            if (gameEvents[i].type == TILE_CLICKED)
            {
                TileMapTile *tile = static_cast<TileMapTile *>(gameEvents[i].tileClickedEvent.tile);
                tile->mTile = tileSet.mTiles[4].get();
            }
        }

        player.update(tileMap.mWidth, tileMap.mHeight);
        player.adjustCamera(camera, tileMap.mWidth, tileMap.mHeight);

        clearGameEvents();

        // Render fps
        int numTilesRendered = tileMap.render(renderer, camera, mouseX, mouseY);
        // int numTilesRendered = 0;
        // tileSet.render(renderer, camera, (800 / 2) - tileSet.mWidth / 2, (640 / 2) - tileSet.mHeight / 2);

        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Avg FPS: " << frameCount / (timerGetTicks(fpsTimer) / 1000.f);
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, camera.w - textTexture->mWidth, 0);

        player.render(renderer, camera);

        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Player (X: " << player.mBox.x << ", Y: " << player.mBox.y << ")";
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, camera.w - textTexture->mWidth, textTexture->mHeight);

        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << std::fixed << "Tiles Rendered: " << numTilesRendered;
        textTexture = Texture::makeTextureFromText(fpsSStream.str(), color, font, renderer);
        textTexture->render(renderer, camera.w - textTexture->mWidth, textTexture->mHeight * 2);

        // Draw
        SDL_RenderPresent(renderer);
        ++frameCount;
    }
}