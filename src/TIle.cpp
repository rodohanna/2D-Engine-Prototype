#include "Tile.h"
#include "Assets.h"
#include "Texture.h"
#include <sstream>
#include "Input.h"

Tile::Tile(SDL_Rect clip, SDL_Rect box, Texture *texture) : mClip(clip), mBox(box), mTexture(texture){};

void Tile::render(SDL_Renderer *renderer, SDL_Rect camera, int x, int y, int scale)
{
    mTexture->render(renderer, x, y, &mClip, scale);
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    int tileWidth = mClip.w * scale, tileHeight = mClip.h * scale;
    if (x < mouseX && x + tileWidth > mouseX && y < mouseY && y + tileHeight > mouseY)
    {
        SDL_Rect rect = {x, y, tileWidth, tileHeight};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x7F);
        SDL_RenderDrawRect(renderer, &rect);
        std::stringstream ss("");
        ss << (x + camera.x) / tileWidth << ", " << (y + camera.y) / tileHeight;
        SDL_Color color = {0x11, 0x11, 0x11, 0xFF};
        auto label = Texture::makeTextureFromText(ss.str(), color, getFont("standard_font"), renderer);
        label->render(renderer, x + ((tileWidth - label->mWidth) / 2), y + ((tileHeight - label->mHeight) / 2));
        if (isInputActive(LEFT_MOUSE_JUST_PRESSED))
        {
            printf("left mouse just pressed\n");
        }
        if (isInputActive(LEFT_MOUSE_PRESSED))
        {
            printf("left mouse pressed\n");
        }
    }
}