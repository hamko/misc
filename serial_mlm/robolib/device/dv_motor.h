/*!
  \file    dv_motor.h
  \ingroup device
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   方向つきモーターの定義と関連関数
 */

#include "install_option.h"
#ifndef __INSTALL_DEVICE__
#error device is not installed
#endif

#ifndef __INCLUDED_DV_MOTOR_H__
#define __INCLUDED_DV_MOTOR_H__

#include "device.h"
#include "libhal.h"
#include "r_stdint.h"

//! モーター構造体
typedef struct {
    hal_motor hal;
    direction_t direction; //!< NEGATIVEならhalと回転方向が逆になる
} motor_t, *p_motor_t;


//! モーターにdutyを設定
/*!
  dutyを負で指定するとmodeとは逆方向に回る
 */
void dv_set_motor(p_motor_t motor, motor_mode mode, int16_t duty);

//! モーターのDutyを符号つきで取得
/*!
  逆方向に回転している場合は負数となる
 */
int16_t dv_get_signed_duty(p_motor_t motor);

//! モーターのモードを取得
/*!
  \retval FORWARD 順方向に回転している
  \retval BACK 逆方向に回転している
 */
motor_mode dv_get_motor_mode(p_motor_t motor);


#endif
