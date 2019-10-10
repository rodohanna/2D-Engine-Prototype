#include "Tile.h"

Tile::Tile(SDL_Rect clip, SDL_Rect box, Texture *texture) : mClip(clip), mBox(box), mTexture(texture){};

void Tile::render(SDL_Renderer *renderer, int x, int y)
{
    mTexture->render(renderer, x, y, &mClip);
}