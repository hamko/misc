/*!
  \file   hal.c
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Fri Feb  8 01:34:02 2008
  
  \brief  HAL設定プログラム。プレフィクスは"HAL"。

  使うデバイスとポートの対応を指定する。
*/

#include "hal.h"
#include "hdwdef.h"
#include "dv_encoder.h"
#include "dv_motor.h"

motor_t s_left_mot, s_right_mot;
encoder_t s_left_enc, s_right_enc;

void entry_motor(p_motor_t motor, hal_motor hal, direction_t dir)
{
    motor->hal=hal;
    motor->direction=dir;
}

void entry_encoder(p_encoder_t encoder, hal_encoder hal, direction_t dir)
{
    encoder->hal=hal;
    encoder->direction=dir;
}

void init_hdwdef(void)
{
    left_mot  = &s_left_mot;
    right_mot = &s_right_mot;
    left_enc  = &s_left_enc;
    right_enc = &s_right_enc;

    entry_motor  (left_mot,  motor0,   POSITIVE);
    entry_motor  (right_mot, motor1,   POSITIVE);
    entry_encoder(left_enc,  encoder0, POSITIVE);
    entry_encoder(right_enc, encoder1, POSITIVE);
}

