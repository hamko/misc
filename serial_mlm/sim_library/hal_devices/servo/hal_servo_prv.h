/*!
  \file   hal_servo_prv.h
  \author TAKISAWA Jun'ichi
  \author ktns
  \author Tatsuru WATANABE
  \date   Fri Dec 14 08:31:23 2007

  \brief  hal_servo.c用プライベートヘッダファイル
  抽象サーボの実宣言を含む。プログラマへの公開は不可。
 */

#ifndef _HAL_SERVO_PRV_H_
#define _HAL_SERVO_PRV_H_

#include "hal_servo.h"
#include "util.h"

//! 抽象サーボオブジェクト
struct hal_servo_tag
{
    short angle;
    BOOLEAN engaged;
};

#endif //_HAL_SERVO_PRV_H_
