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

int gZoomLevel = 2;
int getZoom(int zoomLevel)
{
    int zoom = 1;
    if (zoomLevel == 1)
        zoom = 1;
    else if (zoomLevel == 2)
        zoom = 2;
    else if (zoomLevel == 3)
        zoom = 4;
    return zoom;
}

void setZoomLevel(int zoomLevel, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &camera)
{
    int zoom = getZoom(zoomLevel);
    SDL_RenderSetScale(renderer, zoom, zoom);
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    camera.w = w / zoom;
    camera.h = h / zoom;
    gZoomLevel = zoomLevel;
}

int getZoomLevel()
{
    return gZoomLevel;
}

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
    bool restartFpsTimer = false;
    Uint32 numFramesLastSecond = 60;

    // create tile set
    TileSet tileSet("grass-tiles", 32, 32, 5);
    TileMap tileMap(tileSet, "assets/grass-map.map", 2);

    // create player
    Player player(getTexture("player"));

    // create camera
    SDL_Rect camera = {0, 0, LEVEL_WIDTH, LEVEL_HEIGHT};
    setZoomLevel(1, window, renderer, camera);
    SDL_SetWindowMinimumSize(window, LEVEL_WIDTH, LEVEL_HEIGHT);
    int mouseX = 0, mouseY = 0;
    timerStart(fpsTimer);
    while (!quit)
    {
        if (restartFpsTimer)
        {
            restartFpsTimer = false;
            frameCount = 0;
            timerStart(fpsTimer);
        }
        timerStart(fpsCapTimer);
        clearInput(LEFT_MOUSE_JUST_PRESSED);
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
            {
                int zoomLevel = getZoomLevel();
                switch (e.key.keysym.sym)
                {
                case SDLK_z:
                    ++zoomLevel;
                    break;
                case SDLK_x:
                    --zoomLevel;
                    break;
                }
                if (zoomLevel < 1)
                    zoomLevel = 1;
                if (zoomLevel > 3)
                    zoomLevel = 3;
                setZoomLevel(zoomLevel, window, renderer, camera);
                int zoom = getZoom(zoomLevel);
                int x, y;
                SDL_GetMouseState(&x, &y);
                mouseX = x / zoom;
                mouseY = y / zoom;
            }
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    int zoom = getZoom(getZoomLevel());
                    camera.w = e.window.data1 / zoom;
                    camera.h = e.window.data2 / zoom;
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                int zoom = getZoom(getZoomLevel());
                mouseX = e.button.x / zoom;
                mouseY = e.button.y / zoom;
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

        // Render
        int numTilesRendered = tileMap.render(renderer, camera, mouseX, mouseY);
        // int numTilesRendered = 0;
        // tileSet.render(renderer, camera, (800 / 2) - tileSet.mWidth / 2, (640 / 2) - tileSet.mHeight / 2);

        if (timerGetTicks(fpsTimer) >= 1000)
        {
            restartFpsTimer = true;
            numFramesLastSecond = frameCount;
        }

        fpsSStream.str("");
        fpsSStream.precision(2);
        fpsSStream << "FPS: " << numFramesLastSecond;
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