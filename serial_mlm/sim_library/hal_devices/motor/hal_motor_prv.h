/*!
  \file   hal_motor_prv.h
  \author TAKISAWA Jun'ichi
  \date   Fri Dec 14 08:31:23 2007

  \brief  hal_motor.c用プライベートヘッダファイル
  抽象モータの実宣言を含む。プログラマへの公開は不可。
 */

#ifndef _HAL_MOTOR_PRV_H_
#define _HAL_MOTOR_PRV_H_

#include "hal_motor.h"

//! 抽象モータオブジェクト
struct hal_motor_tag
{
    motor_mode mode;
    unsigned int duty;
};

#endif //_HAL_MOTOR_PRV_H_
