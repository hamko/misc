/*!
  \file    hdwdef.h
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   パラメータ
 */

#ifndef __INCLUDED_PARAMS_H__
#define __INCLUDED_PARAMS_H__

#include "i_math.h"

// device
#define LEFT_ENCODER  left_enc  // 左エンコーダ輪
#define RIGHT_ENCODER right_enc // 右エンコーダ輪
#define LEFT_MOTOR    left_mot  // 左タイヤ
#define RIGHT_MOTOR   right_mot // 右タイヤ
#define LSL_PACKAGE   pmlms     // ラインセンサパッケージ

#define CONTROL_CYCLE 1        // 制御周期 [ms]

// map
#define MAP_ROOT   get_odometry_root()   // 使用するルートマップ
#define MAP_MAP    get_odometry_map()    // 使用するマップ
#define MAP_SUBMAP get_odometry_submap() // 使用するサブマップ
#define STEP_LEFT  7480         // 左輪の1パルスあたり長さ [0.00001mm]
#define STEP_RIGHT 7480         // 右輪の1パルスあたり長さ [0.00001mm]
#define ENCODER_WIDTH 4141      // 左右エンコーダ輪の間隔 [0.1mm]

// wheel
#define K_A 7114               // 加速度係数の逆数 [0.0001mm sec-1 PWM-1] 1422.78 * DT(=5)
#define K_V 23713              // 速度係数の逆数 [0.0001mm sec-1 PWM-1]
#define LEFT_OFFSET 0           // 左タイヤのオフセットDUTY [PWM]
#define RIGHT_OFFSET 0          // 右タイヤのオフセットDUTY [PWM]
#define WHEEL_WIDTH 2650000     // タイヤ間の距離 [0.0001mm]
#define KAKITANI_K 3            // 柿谷制御での角速度式のゲイン
#define STOP_GO_X     10        // go_stop用終了条件 [mm]
#define STOP_GO_Y     100       // go_stop用終了条件 [mm]
#define STOP_GO_THETA 5000      // go_stop用終了条件 [0.0001rad]
#define STOP_TURN_THETA 250     // turn用終了条件 [0.0001rad]
#define A_MAX 40000000              // 最大加速度 [0.0001mm s-2]
#define TURN_A 7                // turnでの角加速度 [rad s-2]
#define MAX_OMEGA pi            // turnでの角速度の最大値 [0.0001rad s-1]
#define ALPHA_MAX 12            // turnでの角加速度の最大値 [rad s-2]

// go_locus
#define GO_LOCUS_R_MIN 500      // go_locusのフィードバックに用いる最小曲率半径 [mm]

//sugimoto
#ifdef __INSTALL_SUGIMOTO__
#define DY_SHIFT_NUM 9
#define MAX_DIFF_DISTANCE 200
#define SEARCH_RANGE 5
#endif

/* マクロ */
#define abs(a) ((a) > 0 ? (a) : -(a))

#define PARTICLE_NUM 100
#define MDAT_NUM 10

#endif
