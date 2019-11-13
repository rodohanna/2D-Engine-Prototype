#include "Player.h"
#include "Assets.h"
#include <stdio.h>

Player::Player(EventBus *eB, const V2 &p, const Color &c) : event_bus(eB), color(c), position(p)
{
    eB->subscribe_to_input_events(this);
    this->texture_index = Assets::get_texture_index("tilesheet-colored");
    this->dimensions = Assets::get_texture_dimensions("tilesheet-colored");
    this->position.x = (800 - this->dimensions.x) / 2;
    this->position.y = (640 - this->dimensions.y) / 2;
}
Player::~Player()
{
    this->event_bus->unsubscribe_to_input_events(this);
}
const double SPEED = 100.0;
bool goRight = true;
void Player::update(double ts)
{
    // int vel = ts * SPEED;
    // if (goRight)
    // {
    //     this->position.x += vel;
    // }
    // else
    // {
    //     this->position.x -= vel;
    // }
    // if (this->position.x > 800 - this->dimensions.x)
    // {
    //     goRight = false;
    // }
    // if (this->position.x < 0)
    // {
    //     goRight = true;
    // }
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.z_index = 1;
    e.data.render_texture_event = {this->texture_index, {}, this->position, 4, false};
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