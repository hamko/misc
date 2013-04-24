/*!
  \file    hdwdef.c
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   robolib用のデバイス定義
 */

#include "hal.h"
#include "hdwdef.h"
//#include "util.h"
#include "device.h"
#include "dv_encoder.h"
#include "dv_motor.h"

motor_t s_left_mot, s_right_mot;
encoder_t s_left_enc, s_right_enc;

static void entry_motor(p_motor_t motor, hal_motor hal, direction_t dir);
static void entry_encoder(p_encoder_t encoder, hal_encoder hal, direction_t dir);

void init_hdwdef(void)
{
    left_mot  = &s_left_mot;
    right_mot = &s_right_mot;
    left_enc  = &s_left_enc;
    right_enc = &s_right_enc;

    entry_motor  (left_mot,  motor0,   POSITIVE);
    entry_motor  (right_mot, motor1,   NEGATIVE);
    entry_encoder(left_enc,  encoder0, POSITIVE);
    entry_encoder(right_enc, encoder1, NEGATIVE);
}

static void entry_motor(p_motor_t motor, hal_motor hal, direction_t dir)
{
    motor->hal=hal;
    motor->direction=dir;
}

static void entry_encoder(p_encoder_t encoder, hal_encoder hal, direction_t dir)
{
    encoder->hal=hal;
    encoder->direction=dir;
}

