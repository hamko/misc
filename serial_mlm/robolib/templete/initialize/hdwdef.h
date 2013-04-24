/*!
  \file    hdwdef.h
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   robolib用のデバイス定義
 */

#ifndef _HDWDEF_H_
#define _HDWDEF_H_

#include "dv_encoder.h"
#include "dv_motor.h"

void init_hdwdef(void);

p_motor_t left_mot, right_mot;
p_encoder_t left_enc, right_enc;

#endif //_HAL_H_
