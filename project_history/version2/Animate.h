#ifndef ANIMATE_h_
#define ANIMATE_h_

#include "GameTypes.h"

struct IntAnimate
{
    IntAnimate();
    void update(double ts, int *num);
    void start(double duration, int start, int end, double delay = 0.0);
    void reset();
    double duration, counter, delay;
    int start_num, end_num;
    bool active, waiting_on_delay;
};

struct PositionAnimate
{
    PositionAnimate();
    void update(double ts, V2 *position);
    void update(double ts, Rect *position);
    void start(int speed, V2 &end);
    void reset();
    V2 end;
    int speed;
    bool active;
};

#endif