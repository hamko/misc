/*!
  \file    dv_encoder.h
  \ingroup device
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   方向つきエンコーダの定義と関連関数
 */

#include "install_option.h"
#ifndef __INSTALL_DEVICE__
#error device is not installed
#endif

#ifndef __INCLUDED_DV_ENCODER_H__
#define __INCLUDED_DV_ENCODER_H__

#include "device.h"
#include "libhal.h"
#include "r_stdint.h"

//! エンコーダ構造体の実体
typedef struct {
    hal_encoder hal;
    direction_t direction; //!< NEGATIVEならhalと回転方向が逆になる
} encoder_t, *p_encoder_t;

//! エンコーダの積算値を取得
int32_t dv_get_encoder_total(p_encoder_t enc);

//! エンコーダの速度を取得
int32_t dv_get_encoder_vel(p_encoder_t enc);


#endif
