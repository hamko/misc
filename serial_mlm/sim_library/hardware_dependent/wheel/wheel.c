#include "device_manager.h"
#include "util.h"
#include "physical_params.h"
#include "runge_kutta.h"
#include "motor_simulation.h"
#include "wheel.h"
#include "world.h"
#include <math.h>

static void presync(hal_device *device);
static void postsync(hal_device *device);
static void renew_machine_vomega(double time, float l_v, float r_v);


static hal_motor rightm = NULL;
static hal_motor leftm = NULL;

static hal_encoder righte = NULL;
static hal_encoder lefte = NULL;

static float _lcurrent = 0;
static float _rcurrent = 0;
//static float init_machine_theta = 0;

hal_motor link_as_right_motor(void)
{
  hal_device *new_device;

  dprintf("\tSetting right motor...");

  new_device = DM_add_device(NULL, leftm==NULL?NULL:postsync);
  clear_motor(&(new_device -> motor));

  dprintf("Done.\n");

  rightm = &(new_device -> motor);
  return rightm;
}

hal_motor link_as_left_motor(void)
{
  hal_device *new_device;

  dprintf("\tSetting left motor...");

  new_device = DM_add_device(NULL, rightm==NULL?NULL:postsync);
  clear_motor(&(new_device -> motor));

  dprintf("Done.\n");

  leftm = &(new_device -> motor);
  return leftm;
}

hal_encoder link_as_left_encoder(void)
{
  hal_device *new_device;

  dprintf( "\tSetting left encoder...");

  new_device = DM_add_device(righte==NULL?NULL:presync, NULL);
  clear_encoder(&(new_device->encoder));

  dprintf( "Done.\n" );

  lefte = &(new_device -> encoder);
  return lefte;
}

hal_encoder link_as_right_encoder(void)
{
  hal_device *new_device;

  dprintf( "\tSetting right encoder...");

  new_device = DM_add_device(lefte==NULL?NULL:presync, NULL);
  clear_encoder(&(new_device->encoder));

  dprintf( "Done.\n" );

  righte = &(new_device -> encoder);
  return righte;
}

// added for GUI
hal_motor get_left_motor(void)
{
    return leftm;
}
hal_motor get_right_motor(void)
{
    return rightm;
}

static exltotal = 0;
static exrtotal = 0;
#define ENC_EXPAND 1000.0
static void presync(hal_device *device)
{
    UNUSED_VARIABLE(device);

    world_pos_t *wpos;
    wpos = get_world_pos();

    double exlvel = (wpos->v - wpos->omega * (double)ENCODER_WIDTH / 2.0) * STEP_DT * ENC_EXPAND / STEP_LEFT;
    double exrvel = (wpos->v + wpos->omega * (double)ENCODER_WIDTH / 2.0) * STEP_DT * ENC_EXPAND / STEP_RIGHT;
    printf("#ENC %ld %ld\n", lefte->velocity, righte->velocity);
    exltotal += exlvel;
    exrtotal += exrvel;

    lefte->velocity = exlvel / ENC_EXPAND;
    righte->velocity = exrvel / ENC_EXPAND;
    lefte->total = exltotal / ENC_EXPAND;
    righte->total = exrtotal / ENC_EXPAND;
/*
    lefte->velocity = (wpos->v - wpos->omega * (double)ENCODER_WIDTH / 2.0) * STEP_DT / STEP_LEFT;
    righte->velocity = (wpos->v + wpos->omega * (double)ENCODER_WIDTH / 2.0) * STEP_DT / STEP_RIGHT;
    printf("#ENC %ld %ld\n", lefte->velocity, righte->velocity);
    lefte->total += lefte->velocity;
    righte->total += righte->velocity;
    */
}


// DUTYからマシンの状態を更新する
// オイラー法でステップサイズを小さくして精度出す力技
static void postsync(hal_device *device)
{
    UNUSED_VARIABLE(device);

    static double total_time=0;
    double temp_time=0;
    float l_v;
    float r_v;

    l_v = leftm->duty * VATTERY_V / 1000;
    r_v = rightm->duty * VATTERY_V / 1000;

    switch (leftm->mode) {
    case FORWARD:
        break;
    case BACK:
        l_v *= -1;
        break;
    case FREE:
    case BRAKE:
        l_v = 0;
        break;
    default:
        break;
    }

    switch (rightm->mode) {
    case FORWARD:
        break;
    case BACK:
        r_v *= -1;
        break;
    case FREE:
    case BRAKE:
        r_v = 0;
        break;
    default:
        break;
    }

    world_pos_t *wpos;
    wpos = get_world_pos();

    /***** sim ****/
    for(temp_time=SIM_DT/2;temp_time<STEP_DT;temp_time+=SIM_DT){
        //速度、角速度を更新。
        renew_machine_vomega(total_time, l_v, r_v);
        //速度、角速度から座標を更新。オドメトリ。
        wpos->theta += wpos->omega*(float)SIM_DT;
        wpos->y += wpos->v*(float)SIM_DT*sin(wpos->omega*(float)SIM_DT/2 + wpos->theta);
        wpos->x += wpos->v*(float)SIM_DT*cos(wpos->omega*(float)SIM_DT/2 + wpos->theta);
        total_time+=SIM_DT;
    }

    dprintf("%f %f %f %f %f\n", wpos->x, wpos->y, wpos->theta, wpos->v, wpos->omega);
    return;
}




/***************
 電圧から速度と角速度をシミュレート
***************/
static void renew_machine_vomega(double time, float l_v, float r_v)
{
    world_pos_t *wpos;
    wpos = get_world_pos();

    double state_vals[4] = { (double)wpos->v / 1000, (double)wpos->omega, (double)_lcurrent, (double)_rcurrent};
    double input[2] = {l_v, r_v};
	
    runge_kutta(motor_simulation, SIM_DT, state_vals, time, VAL_NUM, input);

    wpos->v     = (float)state_vals[0] * 1000;
    wpos->omega = (float)state_vals[1];
    _lcurrent  = (float)state_vals[2];
    _rcurrent = (float)state_vals[3];

    return;
}
