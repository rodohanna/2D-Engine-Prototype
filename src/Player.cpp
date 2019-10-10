#include "Player.h"

const int PLAYER_VEL = 10;

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
        case SDLK_UP:
            mVelocityY -= PLAYER_VEL;
            break;
        case SDLK_DOWN:
            mVelocityY += PLAYER_VEL;
            break;
        case SDLK_LEFT:
            mVelocityX -= PLAYER_VEL;
            break;
        case SDLK_RIGHT:
            mVelocityX += PLAYER_VEL;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            mVelocityY += PLAYER_VEL;
            break;
        case SDLK_DOWN:
            mVelocityY -= PLAYER_VEL;
            break;
        case SDLK_LEFT:
            mVelocityX += PLAYER_VEL;
            break;
        case SDLK_RIGHT:
            mVelocityX -= PLAYER_VEL;
            break;
        }
    }
}

void Player::update(int levelWidth, int levelHeight)
{
    mBox.x += mVelocityX;

    if ((mBox.x < 0) || (mBox.x + mBox.w > levelWidth))
    {
        mBox.x -= mVelocityX;
    }

    mBox.y += mVelocityY;

    if ((mBox.y < 0) || (mBox.y + mBox.h > levelHeight))
    {
        mBox.y -= mVelocityY;
    }
}

void Player::render(SDL_Renderer *renderer, SDL_Rect &camera)
{

    mTexture->render(renderer, mBox.x - camera.x, mBox.y - camera.y, &mClip);
}

void Player::adjustCamera(SDL_Rect &camera, int screenWidth, int screenHeight, int levelWidth, int levelHeight)
{
    camera.x = mBox.x - (camera.w / 2);
    camera.y = mBox.y - (camera.h / 2);

    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.x > levelWidth - camera.w)
    {
        camera.x = levelWidth - camera.w;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.y > levelHeight - camera.h)
    {
        camera.y = levelHeight - camera.h;
    }
}