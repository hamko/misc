/*!
  \file   hal.h
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Tue Dec 11 01:28:54 2007

  \brief  hal.c用ヘッダファイル。
 */

#ifndef _HAL_H_
#define _HAL_H_

#include "libhal.h"
#include "dv_linesensor.h"

extern hal_motor motor0;
extern hal_motor motor1;
/*
extern hal_motor motor2;
extern hal_motor motor3;
extern hal_motor motor4;
extern hal_motor motor5;
extern hal_motor motor6;
*/

extern hal_pscontroller pscontroller0;

extern hal_linesensor linesensor0;
extern hal_linesensor linesensor1;
/*
extern hal_linesensor linesensor2;
extern hal_linesensor linesensor3;
*/

extern hal_switch switch0;
extern hal_switch switch1;
extern hal_switch switch2;
extern hal_switch switch3;
extern hal_switch switch4;
extern hal_switch switch5;
extern hal_switch switch6;
extern hal_switch switch7;
extern hal_switch switch8;
extern hal_switch switch9;
extern hal_switch switch10;
extern hal_switch switch11;
extern hal_switch switch12;
extern hal_switch switch13;
extern hal_switch switch14;
extern hal_switch switch15;

extern hal_switch dipsw1;
extern hal_switch dipsw2;
extern hal_switch dipsw3;
extern hal_switch dipsw4;

extern hal_2008ir irtx;

extern hal_led led0;
extern hal_led led1;
extern hal_led led2;
extern hal_led led3;

extern hal_servo servo0;
extern hal_servo servo1;

extern hal_encoder encoder0;
extern hal_encoder encoder1;
extern hal_encoder encoder2;
extern hal_encoder encoder3;

extern hal_adc adc0;
extern hal_adc adc1;

extern p_ls_material_t plsm[2];
extern p_ls_list_t plsl;
extern p_lsl_package_t pmlms;

#endif //_HAL_H_
