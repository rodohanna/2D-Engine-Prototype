#ifndef FARMTILE_h_
#define FARMTILE_h_

#include "GameEntity.h"
#include "Texture.h"

struct FarmTile : GameEntity
{
    FarmTile(V2 position);
    ~FarmTile();
    bool update(GameState &state);
    void render(SDL_Renderer *renderer, SDL_Rect &camera);
    SDL_Rect mBox;
    Texture *mTexture;
};

#endif