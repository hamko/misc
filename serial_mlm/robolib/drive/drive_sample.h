/*!
  \file    drive_sample.h
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   当分実装予定のないもの。アイディアだけ
 */

//#include "install_option.h"
#ifndef __INSTALL_DRIVE_SAMPLE__
#error drive is not installed
#endif

#ifndef __INCLUDED_DRIVE_SAMPLE_H__
#define __INCLUDED_DRIVE_SAMPLE_H__

/* 2輪用制御器 */
//! 4軸オムニでの二輪の模倣
/*!
  v,omegaをv_x,v_y,omegaに変換してom4_set_v_omegaへ渡す。
  これによって二輪用の制御関数がオムニでも扱えるようになる。
 */
void init_tw_to_om4(void);

//! 夢がひろがりんぐ!!
void init_tw_to_simulator(void);


/* 4軸オムニ用制御器 */
void set_om4_controller(void (*controller)(int32_t v_x, int32_t v_y, int32_t omega));
void om4_set_v_omega(int32_t v_x, int32_t v_y, int32_t omega);
void init_om4_pid(int32_t k_p, int32_t k_i, int32_t k_d, motor_t motor1, motor_t motor2, motor_t motor3, motor_t motor4);


/* 4WD用制御器 */
void set_fwd_controller(void (*controller)(int32_t v, int32_t omega, int32_t steer_theta));
void fwd_set_v_omega(int32_t v, int32_t omega, int32_t steer_theta);
void init_fwd_tdf(int32_t k_a, int32_t k_v, motor_t f_left_motor, motor_t f_right_motor, motor_t r_left_motor, motor_t r_right_motor);


#endif
