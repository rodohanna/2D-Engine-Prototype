#include "Animate.h"
#include "Physics.h"
#include <stdio.h>

IntAnimate::IntAnimate() : active(false), waiting_on_delay(false){};

void IntAnimate::update(double ts, int *num)
{
    if (this->active)
    {
        this->counter += ts;
        if (this->waiting_on_delay)
        {
            if (this->counter >= this->delay)
            {
                this->waiting_on_delay = false;
                this->counter = 0;
            }
            return;
        }
        if (this->counter >= this->duration)
        {
            this->active = false;
            *num = this->end_num;
            return;
        }
        *num = Physics::lerp(this->start_num, this->end_num, this->counter / this->duration);
    }
}

void IntAnimate::start(double duration, int start_num, int end_num, double delay)
{
    this->duration = duration;
    this->delay = delay;
    this->start_num = start_num;
    this->end_num = end_num;
    this->counter = 0;
    this->active = true;
    this->waiting_on_delay = delay > 0;
}

void IntAnimate::reset()
{
    this->duration = 0;
    this->delay = 0;
    this->start_num = 0;
    this->end_num = 0;
    this->counter = 0;
    this->active = false;
    this->waiting_on_delay = false;
}