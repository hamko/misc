/*!
  \file   hal.c
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Fri Feb  8 01:34:02 2008

  \brief  HAL設定プログラム。プレフィクスは"HAL"。

  使うデバイスとポートの対応を指定する。
 */


#include"libhal.h"
#include"libhdw.h"
#include"hal.h"
#include"util.h"
#include "dv_linesensor.h"

hal_motor motor0;
hal_motor motor1;
hal_motor motor2;
hal_motor motor3;
hal_motor motor4;
hal_motor motor5;
hal_motor motor6;

hal_pscontroller pscontroller0;

hal_linesensor linesensor0;
hal_linesensor linesensor1;
/*
hal_linesensor linesensor2;
hal_linesensor linesensor3;
*/

hal_switch switch0;
hal_switch switch1;
hal_switch switch2;
hal_switch switch3;
hal_switch switch4;
hal_switch switch5;
hal_switch switch6;
hal_switch switch7;
hal_switch switch8;
hal_switch switch9;
hal_switch switch10;
hal_switch switch11;
hal_switch switch12;
hal_switch switch13;
hal_switch switch14;
hal_switch switch15;

hal_switch dipsw1;
hal_switch dipsw2;
hal_switch dipsw3;
hal_switch dipsw4;

hal_2008ir irtx;

hal_led led0;
hal_led led1;
hal_led led2;
hal_led led3;

hal_servo servo0;
hal_servo servo1;

hal_encoder encoder0;
hal_encoder encoder1;

hal_adc adc0;
hal_adc adc1;

static ls_material_t lsm[2];
p_ls_material_t plsm[2];
static ls_list_t slsl;
p_ls_list_t plsl;
static lsl_package_t smlms;
p_lsl_package_t pmlms;
static p_ls_list_t arr_lsl[1];

void HAL_initialize(void)
{
    motor0 = link_as_left_motor();
    motor1 = link_as_right_motor();
    /*
    motor2 = link_null_as_motor();
    motor3 = link_null_as_motor();
    motor4 = link_null_as_motor();
    */

    pscontroller0 = link_null_as_pscontroller();

    encoder0 = link_as_left_encoder();
    encoder1 = link_as_right_encoder();

    // ラインセンサ
    linesensor0 = link_as_linesensor0();
    linesensor1 = link_as_linesensor1();
    // 各種構造体の初期化
    plsl = &slsl;
    arr_lsl[0] = plsl;
    pmlms = &smlms;
    plsm[0] = &lsm[0]; // = lsm
    plsm[1] = &lsm[1]; // = lsm+1
    // ラインセンサ板の設定
    set_ls_material(lsm, linesensor0, POSITIVE, 8, 100, 100);
    set_ls_material(lsm+1, linesensor1, POSITIVE, 8, 100, 100);
    // ラインセンサリストの設定
    // 二番目の引数はラインセンサ単独構造体ポインタ配列先頭
    set_ls_list(plsl, plsm, 2, 200, 0, 0);
    pmlms->device = arr_lsl;
    pmlms->num = 1;

    adc0 = link_null_as_adc();
    adc1 = link_null_as_adc();

    switch0 = link_true_as_switch();
    switch1 = link_false_as_switch();
    switch2 = link_true_as_switch();
    switch3 = link_true_as_switch();
    switch4 = link_true_as_switch();
    switch5 = link_true_as_switch();
    switch6 = link_true_as_switch();
    switch7 = link_true_as_switch();

    led0 = link_null_as_led();
    led1 = link_null_as_led();
    led2 = link_null_as_led();
    led3 = link_null_as_led();

    servo0 = link_null_as_servo();
    servo1 = link_null_as_servo();
}
