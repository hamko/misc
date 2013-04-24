/*!
  \file   hal_motor.c
  \author TAKISAWA <junichi@Hyperion.private.net>
  \date   Tue Dec  4 18:17:04 2007

  \brief  HAL用モータオブジェクト

  抽象化されたモータオブジェクトとそれに対する操作を行う関数群を定義する。
 */

#include"hal_motor_prv.h"

void clear_motor(hal_motor motor)
{
    //! モータを初期状態、FREEモードDuty0へ設定する。
    set_motor(motor, FREE, 0);
}

void set_motor(hal_motor motor, motor_mode mode, unsigned int duty)
{
    //! 指定されたモータの動作モードと
    set_motor_mode(motor, mode);
    //! Dutyを設定する。
    set_motor_duty(motor, duty);
}

void set_motor_mode(hal_motor motor, motor_mode mode)
{
    //! 指定されたモータの動作モードを設定する。
    motor -> mode = mode;
}

void set_motor_duty(hal_motor motor, unsigned int duty)
{
    //! 指定したモータのDutyを設定する。
    //! 指定可能な値を超える値が指定された場合は指定可能な最大値へ変更される。
    //! 指定可能な最大値はMOTOR_MAX_DUTYとして定義されている。
    if(duty > MOTOR_MAX_DUTY -1)
    {
        duty = MOTOR_MAX_DUTY -1;
    }
    motor -> duty = duty;
}

motor_mode get_motor_mode(hal_motor motor)
{
    //! 指定されたモータの現行モードを返す。
    return motor -> mode;
}

unsigned int get_motor_duty(hal_motor motor)
{
    //! 指定されたモータの現行Dutyを返す。
    return motor -> duty;
}
