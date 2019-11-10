#include "Player.h"
#include "Assets.h"
#include <stdio.h>

Player::Player(EventBus *eB, const V2 &p, const Color &c) : event_bus(eB), color(c), position(p)
{
    eB->subscribe_to_input_events(this);
    this->texture_index = Assets::get_texture_index("apple");
    this->dimensions = Assets::get_texture_dimensions("apple");
}
Player::~Player()
{
    this->event_bus->unsubscribe_to_input_events(this);
}
const double SPEED = 350.0;
bool goRight = true;
void Player::update(double ts)
{
    int vel = ts * SPEED;
    if (goRight)
    {
        this->position.x += vel;
    }
    else
    {
        this->position.x -= vel;
    }
    if (this->position.x > 800 - this->dimensions.x)
    {
        goRight = false;
    }
    if (this->position.x < 0)
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
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.data.render_texture_event = {this->texture_index, this->position};
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