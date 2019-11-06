#include "Player.h"

Player::Player(EventBus *eB, const FRect &r, const Color &c) : eventBus(eB), box(r), color(c)
{
    eB->subscribeToInputEvents(this);
}
Player::~Player()
{
    this->eventBus->unsubscribeToInputEvents(this);
}
const double SPEED = 5.0;
void Player::update()
{
    RenderEvent e;
    e.type = RENDER_RECTANGLE;
    e.data.renderRectangleEvent.prev = this->box;
    if (actions[PlayerActions::MOVE_UP])
    {
        this->box.y -= SPEED;
    }
    if (actions[PlayerActions::MOVE_DOWN])
    {
        this->box.y += SPEED;
    }
    if (actions[PlayerActions::MOVE_LEFT])
    {
        this->box.x -= SPEED;
    }
    if (actions[PlayerActions::MOVE_RIGHT])
    {
        this->box.x += SPEED;
    }
    e.data.renderRectangleEvent.now = this->box;
    e.data.renderRectangleEvent.color = this->color;
    eventBus->publishRenderEvent(e);
}
void Player::handleInputEvents(const InputEvent *inputEvents, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        InputEvent e = inputEvents[i];
        if (e.type == KEY_DOWN)
        {
            if (e.data.keyEvent.key == W_KEY)
            {
                actions[PlayerActions::MOVE_UP] = true;
            }
            else if (e.data.keyEvent.key == S_KEY)
            {
                actions[PlayerActions::MOVE_DOWN] = true;
            }
            else if (e.data.keyEvent.key == A_KEY)
            {
                actions[PlayerActions::MOVE_LEFT] = true;
            }
            else if (e.data.keyEvent.key == D_KEY)
            {
                actions[PlayerActions::MOVE_RIGHT] = true;
            }
        }
        else if (e.type == KEY_UP)
        {
            if (e.data.keyEvent.key == W_KEY)
            {
                actions[PlayerActions::MOVE_UP] = false;
            }
            else if (e.data.keyEvent.key == S_KEY)
            {
                actions[PlayerActions::MOVE_DOWN] = false;
            }
            else if (e.data.keyEvent.key == A_KEY)
            {
                actions[PlayerActions::MOVE_LEFT] = false;
            }
            else if (e.data.keyEvent.key == D_KEY)
            {
                actions[PlayerActions::MOVE_RIGHT] = false;
            }
        }
    }
}