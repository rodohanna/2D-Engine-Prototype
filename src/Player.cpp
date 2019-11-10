#include "Player.h"
#include <stdio.h>

Player::Player(EventBus *eB, const Rect &r, const Color &c) : event_bus(eB), box(r), color(c)
{
    eB->subscribe_to_input_events(this);
}
Player::~Player()
{
    this->event_bus->unsubscribe_to_input_events(this);
}
const double SPEED = 350.0;
bool goRight = true;
void Player::update(double ts)
{
    RenderEvent e;
    e.type = RENDER_RECTANGLE;
    int vel = ts * SPEED;
    if (goRight)
    {
        this->box.x += vel;
    }
    else
    {
        this->box.x -= vel;
    }
    if (this->box.x > 700)
    {
        goRight = false;
    }
    if (this->box.x < 0)
    {
        goRight = true;
    }
    // if (actions[PlayerActions::MOVE_UP])
    // {
    //     this->box.y -= vel;
    // }
    // if (actions[PlayerActions::MOVE_DOWN])
    // {
    //     this->box.y += vel;
    // }
    // if (actions[PlayerActions::MOVE_LEFT])
    // {
    //     this->box.x -= vel;
    // }
    // if (actions[PlayerActions::MOVE_RIGHT])
    // {
    //     this->box.x += vel;
    // }
    e.data.renderRectangleEvent.box = this->box;
    e.data.renderRectangleEvent.color = this->color;
    event_bus->publish_render_event(e);
}
void Player::handle_input_events(const InputEvent *inputEvents, size_t length)
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