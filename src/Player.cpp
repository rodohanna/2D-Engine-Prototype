#include "Player.h"
#include <algorithm>
#include "Input.h"
#include "EventBus.h"
#include "GameEvent.h"

const int PLAYER_VEL = 5;

Player::Player(Texture *texture) : mTexture(texture)
{
    mClip = {2, 6, 30, 40};
    mBox.x = 10;
    mBox.y = 10;
    mBox.w = mClip.w - mClip.x;
    mBox.h = mClip.h - mClip.y;
    mVelocityX = 0;
    mVelocityY = 0;
};

Player::~Player(){};

void Player::handleInput(GameState &state)
{
    if (isInputActive(W_KEY_DOWN))
    {
        mVelocityY -= PLAYER_VEL;
    }
    else if (isInputActive(W_KEY_UP))
    {
        mVelocityY += PLAYER_VEL;
    }

    if (isInputActive(S_KEY_DOWN))
    {
        mVelocityY += PLAYER_VEL;
    }
    else if (isInputActive(S_KEY_UP))
    {
        mVelocityY -= PLAYER_VEL;
    }

    if (isInputActive(A_KEY_DOWN))
    {
        mVelocityX -= PLAYER_VEL;
    }
    else if (isInputActive(A_KEY_UP))
    {
        mVelocityX += PLAYER_VEL;
    }

    if (isInputActive(D_KEY_DOWN))
    {
        mVelocityX += PLAYER_VEL;
    }
    else if (isInputActive(D_KEY_UP))
    {
        mVelocityX -= PLAYER_VEL;
    }

    if (isInputActive(LEFT_MOUSE_JUST_PRESSED))
    {
        TillSoilEvent *e = new TillSoilEvent();
        e->eventType = TILL_SOIL;
        e->coords = state.mouseCoords;
        printf("player: %d %d\n", state.mouseCoords.x, state.mouseCoords.y);
        printf("player 2: %d %d\n", e->coords.x, e->coords.y);
        publish(std::unique_ptr<TillSoilEvent>(e));
    }
}

bool Player::update(GameState &state)
{
    handleInput(state);
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

    adjustCamera(state.camera, state.mapDimensions.x, state.mapDimensions.y);
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