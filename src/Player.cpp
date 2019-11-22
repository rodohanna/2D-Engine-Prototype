#include "Player.h"
#include "Assets.h"
#include "Window.h"
#include <stdio.h>

Player::Player(EventBus *eB, const V2 &p, const Color &c) : event_bus(eB), color(c), position(p)
{
    eB->subscribe_to_input_events(this);
    this->texture_index = Assets::get_texture_index("tilesheet-colored");
    this->dimensions = Assets::get_texture_dimensions("tilesheet-colored");
    this->position.x = 0;
    this->position.y = 0;
}
Player::~Player()
{
    this->event_bus->unsubscribe_to_input_events(this);
}
const double SPEED = 500.0;
void Player::update(double ts)
{
    int vel = SPEED * ts;
    if (actions[PlayerActions::MOVE_UP])
    {
        this->position.y -= vel;
    }
    if (actions[PlayerActions::MOVE_DOWN])
    {
        this->position.y += vel;
    }
    if (actions[PlayerActions::MOVE_LEFT])
    {
        this->position.x -= vel;
    }
    if (actions[PlayerActions::MOVE_RIGHT])
    {
        this->position.x += vel;
    }
    Rect *old_camera = Window::get_camera();
    Window::set_camera({this->position.x, this->position.y, old_camera->w, old_camera->h});
}
void Player::handle_input_events(const InputEvent *inputEvents, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        InputEvent e = inputEvents[i];
        if (e.type == KEY_DOWN)
        {
            if (e.data.key_event.key == W_KEY)
            {
                actions[PlayerActions::MOVE_UP] = true;
            }
            else if (e.data.key_event.key == S_KEY)
            {
                actions[PlayerActions::MOVE_DOWN] = true;
            }
            else if (e.data.key_event.key == A_KEY)
            {
                actions[PlayerActions::MOVE_LEFT] = true;
            }
            else if (e.data.key_event.key == D_KEY)
            {
                actions[PlayerActions::MOVE_RIGHT] = true;
            }
        }
        else if (e.type == KEY_UP)
        {
            if (e.data.key_event.key == W_KEY)
            {
                actions[PlayerActions::MOVE_UP] = false;
            }
            else if (e.data.key_event.key == S_KEY)
            {
                actions[PlayerActions::MOVE_DOWN] = false;
            }
            else if (e.data.key_event.key == A_KEY)
            {
                actions[PlayerActions::MOVE_LEFT] = false;
            }
            else if (e.data.key_event.key == D_KEY)
            {
                actions[PlayerActions::MOVE_RIGHT] = false;
            }
        }
    }
}