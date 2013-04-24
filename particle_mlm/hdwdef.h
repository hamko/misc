/*!
  \file   hal.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Tue Dec 11 01:28:54 2007
  
  \brief  hal.c用ヘッダファイル。
*/

#ifndef _HDWDEF_H_
#define _HDWDEF_H_

#include "libhal.h"
#include "dv_encoder.h"
#include "dv_motor.h"
#include "dv_linesensor.h"

p_motor_t left_mot, right_mot;
p_encoder_t left_enc, right_enc;
p_lsl_package_t ls_pack;

void init_hdwdef(void);


#endif //_HDWDEF_H_
