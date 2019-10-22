#include "Tile.h"

Tile::Tile(SDL_Rect clip, SDL_Rect box, Texture *texture) : mClip(clip), mBox(box), mTexture(texture){};

void Tile::render(SDL_Renderer *renderer, SDL_Rect camera, int x, int y, int scale)
{
    mTexture->render(renderer, x, y, &mClip, scale);
}