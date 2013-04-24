/*!
  \file    duty_odometry.h
  \ingroup duty_odometry
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/09

  \brief   Dutyの積算で簡易的なマップを生成
  \see     duty_odometry.c map.c
 */

#include "install_option.h"
#ifndef __INSTALL_DUTY_ODOMETRY__
#error duty_odometry is not installed
#endif

#ifndef __INCLUDED_DUTY_ODOMETRY_H__
#define __INCLUDED_DUTY_ODOMETRY_H__

#include "device.h"
#include "dv_motor.h"
#include "map.h"
#include "r_stdint.h"

//! 初期化用パラメータ
typedef struct {
    p_motor_t left_motor;  //!< 左エンコーダ輪
    p_motor_t right_motor; //!< 右エンコーダ輪
    int32_t k_a;         //!< 加速度係数の逆数
    int32_t k_v;         //!< 速度係数の逆数
    int32_t r;           //!< 摩擦項
    int32_t width;       //!< 左右のタイヤ間隔
} duty_odometry_t;

//! Dutyオドメトリの初期化
/*
  オドメトリ更新を関数リストに登録し、3層のマップをマップリストに登録
  これを呼び出した後は、get_map_pos(get_duty_odometry_***())で座標を取得できる。
 */
void init_duty_odometry(duty_odometry_t params);

//! ルートマップへのポインタを取得
p_map_t get_duty_odometry_root(void);

//! マップへのポインタを取得
p_map_t get_duty_odometry_map(void);

//! サブマップへのポインタを取得
p_map_t get_duty_odometry_submap(void);

#endif
