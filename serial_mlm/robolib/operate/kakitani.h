/*!
  \file    kakitani.h
  \ingroup kakitani
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/12

  \brief   柿谷制御
 */

#include "install_option.h"
#ifndef __INSTALL_KAKITANI__
#error kakitani is not installed
#endif

#ifndef __INCLUDED_KAKITANI_H__
#define __INCLUDED_KAKITANI_H__

#include "map.h"
#include "r_stdint.h"

typedef struct {
    int k;  //!< 角速度式のゲイン。理論値2、推奨値3。
    int32_t stop_go_x; //!< go_stop用終了条件(単位:mm) 推奨値10。
    int32_t stop_go_y; //!< go_stop用終了条件(単位:mm) 推奨値100。
    int32_t stop_go_theta;  //!< go_stop用終了条件(単位:1/10000 rad) 推奨値5000。
    int32_t stop_turn_theta; //!< turn用終了条件(単位:1/10000 rad) 推奨値250。
    int32_t a_max; //!< 最大加速度(単位:mm/s^2) 推奨値4000。
    int32_t turn_a; //!< turnでの角加速度。推奨値7。
    int32_t max_omega; //!< turnでの角速度の最大値(0.0001rad/s^2) 推奨値pi。
    int32_t alpha_max; //!< turnでの角加速度の最大値(rad/s^2) 推奨値12。
    p_map_t map;  //!< 利用するマップ
    p_map_t submap;  //!< 利用するサブマップ
} kakitani_t;

void init_kakitani(kakitani_t params);

// 標準柿谷指令関数
//! カーブを描きながら走行する
/*!
  \param x,y,theta 目標地点、方向
  \param v 走行速度
  \param r 最小曲率半径
 */
void go_curve(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r);
void go_curve_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r);

//! カーブを描きながら走行し、目標地点で止まる
void go_stop(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a);
void go_stop_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a);

//! 心地旋回する
void turn(int32_t theta);
void turn_with(void (*block)(void), int32_t theta);

// 拡張柿谷指令関数
//! go_curveのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_curve(int32_t x, int32_t y, int32_t theta, int32_t speed, int32_t r);
void go_back_curve_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t speed, int32_t r);

//! go_stopのバック版。
/*!
  \warning xの符号に注意すること。
 */
void go_back_stop(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a);
void go_back_stop_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t r, int32_t a);

//! 指定した角度になるように心地旋回する
void turn_to(int32_t theta);
void turn_to_with(void (*block)(void), int32_t theta);

//! 指定した角度になるように心地旋回する
/*!
  通常時はブレーキ、外部から回転させられると戻す。
 */
void turn_stay(int32_t theta);

//! go_curveに加え、終端付近で目標終端速度へ向けて加減速する
void go_accele(int32_t x, int32_t y, int32_t theta, int32_t v, int32_t v_term, int32_t r, int32_t a);
void go_accele_with(void (*block)(void), int32_t x, int32_t y, int32_t theta, int32_t v, int32_t v_term, int32_t r, int32_t a);

//! 直進性を確保しつつ、指定した一定のDutyで走ろうとする
void go_const(int32_t c_F, int32_t r, int32_t control_time);
void go_const_with(void (*block)(void), int32_t c_F, int32_t r, int32_t control_time);

#endif
