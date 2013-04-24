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

//! 初期化用パラメータ
typedef struct {
    p_encoder_t left_encoder;  //!< 左エンコーダ輪
    p_encoder_t right_encoder; //!< 右エンコーダ輪
    int32_t step_left;       //!< 左輪の1パルスあたり長さ (単位:0.00001mm)
    int32_t step_right;      //!< 右輪の1パルスあたり長さ (単位:0.00001mm)
    int32_t encoder_width;   //!< 左右エンコーダ輪の間隔 (単位:0.1mm)
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
