/*!
  \file   hal_servo.c
  \author TAKISAWA <junichi@Hyperion.private.net>
  \author ktns
  \date   Tue Dec  4 18:17:04 2007

  \brief  HAL用サーボオブジェクト

  抽象化されたサーボオブジェクトとそれに対する操作を行う関数群を定義する。
 */

#include "hal_servo_prv.h"

void engage_servo(hal_servo servo)
{
    servo->engaged = 1;
}

void release_servo(hal_servo servo)
{
    servo->engaged = 0;
}

void set_servo_angle(hal_servo servo,short angle)
{
    //! 指定したサーボの角度を設定する。
    //! 指定可能な値はshortの範囲と等しいため、値のチェックはしない
    servo->angle = angle;
}

short get_servo_angle(hal_servo servo)
{
    //! 指定されたサーボの現行angleを返す。
    return servo -> angle;
}
