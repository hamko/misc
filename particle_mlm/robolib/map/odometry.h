/*!
  \file    odometry.h
  \ingroup map
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   オドメトリによるマップ生成
 */

#include "install_option.h"
#ifndef __INSTALL_MAP__
#error map is not installed
#endif

#ifndef __INCLUDED_ODOMETRY_H__
#define __INCLUDED_ODOMETRY_H__

#include "device.h"
#include "dv_encoder.h"
#include "map.h"
#include "r_stdint.h"
#include "dv_linesensor.h"

#define PARTICLE_NUM 100
#define PHOTOSENSOR_NUM (8*2) // 8素子のラインセンサが2枚

#define PARTICLE_INIT_X_RANGE (100*UNIT_MM)
#define PARTICLE_INIT_Y_RANGE (100*UNIT_MM)
#define PARTICLE_INIT_THETA_RANGE (UNIT_RAD / 10) // 6度くらい
#define HALF_LINE_WIDTH (15*UNIT_MM)

//! 初期化用パラメータ
typedef struct {
    p_encoder_t left_encoder;  //!< 左エンコーダ輪
    p_encoder_t right_encoder; //!< 右エンコーダ輪
    int32_t step_left;       //!< 左輪の1パルスあたり長さ (単位:0.00001mm)
    int32_t step_right;      //!< 右輪の1パルスあたり長さ (単位:0.00001mm)
    int32_t encoder_width;   //!< 左右エンコーダ輪の間隔 (単位:0.1mm)
    // 0: rot coeff for rots, 1: trans coeff for turn, 2: trans coeff for trans, 3: 2-norm(rot1, rot2) coeff for trans
    // 単位はもう何か面倒くさいので、UNIT_MM = UNIT_RADを前提します。　
    float var_coeff[4];
    p_lsl_package_t lsl_pack; //!< ラインセンサリストのパッケージ
    //パッケージ中の素子をすべて使う
} odometry_t;

//! オドメトリの初期化
/*
  オドメトリ更新を関数リストに登録し、3層のマップをマップリストに登録
 */
void init_odometry(odometry_t params);

//! オドメトリ座標を持つマップへのポインタを取得
p_map_t get_odometry_root(void);

//! オドメトリから生成したマップへのポインタを取得
p_map_t get_odometry_map(void);

//! オドメトリから生成したサブマップへのポインタを取得
p_map_t get_odometry_submap(void);


#endif
