/*!
  \file    drive_tw.h
  \ingroup drive_tw
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/12

  \brief   二輪マシン用駆動関数

  v, omegaをDUTYに変換し、モーターを駆動する\n
  initを呼んだ制御器を用いてtw_set_v_omegaで駆動する。
  例えばgo_curveからは、計算したdest_v, dest_omegaがtw_set_v_omegaに渡される
 */

#include "install_option.h"
#ifndef __INSTALL_DRIVE_TW__
#error drive_tw is not installed
#endif

#ifndef __INCLUDED_DRIVE_TW_H__
#define __INCLUDED_DRIVE_TW_H__

#include "device.h"
#include "dv_motor.h"
#include "map.h"
#include "r_stdint.h"

//! 2自由度用初期化パラメータ(tdf = two degree of freedom)
typedef struct {
    int32_t k_a;         //!< フィードバックゲイン
    int32_t k_v;         //!< フィードフォワードゲイン
    int32_t offset_l;    //!< 左タイヤのオフセットDUTY
    int32_t offset_r;    //!< 右タイヤのオフセットDUTY
    int32_t width;       //!< タイヤ間の距離(単位:0.0001mm)
    p_motor_t left_motor;  //!< 左タイヤ
    p_motor_t right_motor; //!< 右タイヤ
    p_map_t map;         //!< 速度取得のためのマップ
} tw_tdf_t;

//! PID用初期化パラメータ
typedef struct {
    int32_t k_p;         //!< P係数
    int32_t k_i;         //!< I係数
    int32_t k_d;         //!< D係数
    int32_t offset_l;    //!< 左タイヤのオフセットDUTY
    int32_t offset_r;    //!< 右タイヤのオフセットDUTY
    int32_t width;       //!< タイヤ間の距離(単位:0.0001mm)
    p_motor_t left_motor;  //!< 左タイヤ
    p_motor_t right_motor; //!< 右タイヤ
    p_map_t map;         //!< 速度取得のためのマップ
} tw_pid_t;

typedef struct {
    int32_t k_f_a;         // Fフィードバックゲイン
    int32_t k_f_v;         // Fフィードフォワードゲイン
    int32_t dt_f;          // FのDT
    int32_t k_m_a;         // Mフィードバックゲイン
    int32_t k_m_v;         // Mフィードフォワードゲイン
    int32_t dt_m;          // MのDT
    int32_t offset_l;    // 左タイヤのオフセットDUTY
    int32_t offset_r;    // 右タイヤのオフセットDUTY
    p_motor_t left_motor;  // 左タイヤ
    p_motor_t right_motor; // 右タイヤ
    p_map_t map;         // 速度取得のためのマップ
} tw_fm_t;

// tdf+I
typedef struct {
    int32_t k_a;         // フィードバックゲイン
    int32_t k_v;         // フィードフォワードゲイン
    int32_t k_i;         // I成分ゲイン 
    int32_t offset_l;    // 左タイヤのオフセットDUTY
    int32_t offset_r;    // 右タイヤのオフセットDUTY
    int32_t width;       // タイヤ間の距離(単位:0.0001mm)
    p_motor_t left_motor;  // 左タイヤ
    p_motor_t right_motor; // 右タイヤ
    int16_t default_shift_num;  //オーバーフロー防止用デフォルトシフト
    p_map_t map;         // 速度取得のためのマップ
} tw_tdf_pi_t;


//! 二輪用の制御器を設定
/*
  基本的にinit_tw_*内でのみ使用する。
  \param controller 二輪用制御器関数へのポインタ
 */
void set_tw_controller(void (*controller)(int32_t v, int32_t omega));

//! 二輪マシンのタイヤを駆動
/*!
  set_tw_controllerで設定された制御器にv, omegaを受け渡す
  足回りの制御関数はこの関数に対して出力を行う。
 */
void tw_set_v_omega(int32_t v, int32_t omega);

//! 二輪マシンの停止(FREE)
void tw_free(void);

//! 二輪マシンの停止(BRAKE)
void tw_brake(void);

//! 2自由度系の制御器による二輪の駆動
void init_tw_tdf(tw_tdf_t params);

//! PID制御での二輪の駆動
void init_tw_pid(tw_pid_t params);

// F/M制御器
void init_tw_fm(tw_fm_t params);

// tdf+I
void init_tw_tdf_pi(tw_tdf_pi_t params);

//! パラメータとして指定した右タイヤ用モータへのポインタを取得
p_motor_t get_tw_right(void);
//! パラメータとして指定した左タイヤ用モータへのポインタを取得
p_motor_t get_tw_left(void);

// 目標速度を返す
int32_t get_dest_v(void);
// 目標角速度を返す
int32_t get_dest_omega(void);

#endif
