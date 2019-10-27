#include "Plot.h"
#include "Assets.h"
#include "Input.h"
#include <sstream>

Plot::Plot(TileMap *t) : mTileMap(t)
{
    mDebugRect = {0, 0, 0, 0};
}
Plot::~Plot() {}
bool Plot::update(GameState &state)
{
    TileMapTile *tile = mTileMap->getHoveredTile(state);
    if (tile == NULL)
    {
        mDebugRect = {0, 0, 0, 0};
        return false;
    }
    if (isInputActive(LEFT_MOUSE_JUST_PRESSED))
    {
        tile->mTile = mTileMap->mTileSet->mTiles[0].get();
    }
    mDebugRect = {tile->mBox.x - state.camera.x, tile->mBox.y - state.camera.y, tile->mBox.w, tile->mBox.h};
    return true;
};
void Plot::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x7F);
    SDL_RenderDrawRect(renderer, &mDebugRect);
    if (mDebugRect.w == 0 || mDebugRect.h == 0)
    {
        return;
    }
    std::stringstream ss("");
    ss << (mDebugRect.x + camera.x) / mDebugRect.w << ", " << (mDebugRect.y + camera.y) / mDebugRect.h;
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    auto label = Texture::makeTextureFromText(ss.str(), color, getFont("standard_font"), renderer);
    label->render(renderer, mDebugRect.x + ((mDebugRect.w - label->mWidth) / 2), mDebugRect.y + ((mDebugRect.h - label->mHeight) / 2));
};