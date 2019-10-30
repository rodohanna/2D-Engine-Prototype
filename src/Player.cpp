#include "Player.h"
#include <algorithm>

const int PLAYER_VEL = 5;

Player::Player(Texture *texture) : mTexture(texture)
{
    mClip = {2, 6, 30, 40};
    mBox.x = 10;
    mBox.y = 10;
    mBox.w = mClip.w - mClip.x;
    mBox.h = mClip.h - mClip.y;
};

void Player::handleInput(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_w:
            mVelocityY -= PLAYER_VEL;
            break;
        case SDLK_s:
            mVelocityY += PLAYER_VEL;
            break;
        case SDLK_a:
            mVelocityX -= PLAYER_VEL;
            break;
        case SDLK_d:
            mVelocityX += PLAYER_VEL;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_w:
            mVelocityY += PLAYER_VEL;
            break;
        case SDLK_s:
            mVelocityY -= PLAYER_VEL;
            break;
        case SDLK_a:
            mVelocityX += PLAYER_VEL;
            break;
        case SDLK_d:
            mVelocityX -= PLAYER_VEL;
            break;
        }
    }
}

bool Player::update(GameState &state)
{
    mBox.x += mVelocityX;

    if ((mBox.x < 0) || (mBox.x + mBox.w > state.mapDimensions.x))
    {
        mBox.x -= mVelocityX;
    }

    mBox.y += mVelocityY;

    if ((mBox.y < 0) || (mBox.y + mBox.h > state.mapDimensions.y))
    {
        mBox.y -= mVelocityY;
    }
    return true;
}

void Player::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    mTexture->render(renderer, mBox.x - camera.x, mBox.y - camera.y, &mClip);
}

void Player::handleEvent(GameEvent *e, GameState *state)
{
    return;
}

void Player::adjustCamera(SDL_Rect &camera, int levelWidth, int levelHeight)
{
    camera.x = mBox.x + (mBox.w / 2) - (camera.w / 2);
    camera.y = mBox.y + (mBox.h / 2) - (camera.h / 2);

    // if (camera.x < 0)
    // {
    //     camera.x = 0;
    // }
    // if (camera.x > levelWidth - camera.w)
    // {
    //     camera.x = levelWidth - camera.w;
    // }
    // if (camera.y < 0)
    // {
    //     camera.y = 0;
    // }
    // if (camera.y > levelHeight - camera.h)
    // {
    //     camera.y = levelHeight - camera.h;
    // }
}