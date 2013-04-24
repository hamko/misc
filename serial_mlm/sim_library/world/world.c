#include "world.h"

static world_pos_t _wpos;		/* rad/s */

world_pos_t* get_world_pos(void) 
{
    return &_wpos;
}

void set_world_pos(float x, float y, float theta)
{
    _wpos.x = x;
    _wpos.y = y;
    _wpos.theta = theta;
}

void set_world_pos_all(float x, float y, float theta, float v, float omega)
{
    _wpos.x = x;
    _wpos.y = y;
    _wpos.theta = theta;
    _wpos.v = v;
    _wpos.omega = omega;
}
