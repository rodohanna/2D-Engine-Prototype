#ifndef PLAYER_h_
#define PLAYER_h_
#include <SDL2/SDL.h>
#include "GameEntity.h"
#include "GameState.h"
#include "Texture.h"
struct Player : public GameEntity
{
    Player(Texture *texture);
    void handleInput(SDL_Event &e);
    void adjustCamera(SDL_Rect &camera, int levelWidth, int levelHeight);
    void render(SDL_Renderer *renderer, SDL_Rect &camera);
    bool update(GameState &state);
    int mPositionX, mPositionY, mVelocityX, mVelocityY;
    SDL_Rect mBox;
    SDL_Rect mClip;
    Texture *mTexture;
};

#endif