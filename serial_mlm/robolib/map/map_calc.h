/*!
  \file    map_calc.h
  \ingroup map
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   マップ操作に関する演算
 */

#include "install_option.h"
#ifndef __INSTALL_MAP__
#error map is not installed
#endif

#ifndef __INCLUDED_MAP_CALC_H__
#define __INCLUDED_MAP_CALC_H__

#include "map.h"
#include "r_stdint.h"

//! マップを平行移動させる
/*!
  \param map 移動させるマップ
  \param origin 移動ベクトルの座標系
  \param x      移動ベクトルx座標
  \param y      移動ベクトルy座標
 */
void transfer_map(p_map_t map, p_map_t origin, int32_t x, int32_t y);

//! マップ・サブマップを現在位置中心に回転
/*!
  \param map 回転するマップ
  \param theta 現在座標を中心とした回転角
 */
void rotate_map_here(p_map_t map, int32_t theta);

//! マップを指定座標中心に回転
/*!
  \param map 回転するマップ
  \param origin 回転中心の座標系
  \param x 回転中心のx座標
  \param y 回転中心のy座標
  \param theta 回転角度
 */
void rotate_map(p_map_t map, p_map_t origin, int32_t x, int32_t y, int32_t theta);


//! 現在位置・角度が指定位置・角度になるようにマップを修正
/*!
  \param map 修正するマップ
  \param origin 指定座標の座標系
  \param x 指定する位置のx座標
  \param y 指定する位置のy座標
  \param theta 指定する角度
 */
void set_map_pos(p_map_t map, p_map_t origin, int32_t x, int32_t y, int32_t theta);

#endif
