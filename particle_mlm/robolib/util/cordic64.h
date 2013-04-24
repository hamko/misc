/*!
  \file    cordic64.h
  \ingroup cordic
  \author  Shimada <takumi@qc5.so-net.ne.jp>
  \date    2011/12/21

  \brief   CORDIC法をint64_t型を使うことでより高精度な値を返す。unit_system.hで宣言されたUNIT_MM_LとUNIT_RAD_Lで倍率をする。
 */

#ifndef __INCLUDED_CORDIC64_H__
#define __INCLUDED_CORDIC64_H__

//#include <libkairo.h>

#define I_ARCTAN_MAX_L +1570796
#define I_ARCTAN_MIN_L -1570796
//! CORDIC法によるsin
int32_t c_sinl(int32_t arg);

//! CORDIC法によるcos
int32_t c_cosl(int32_t arg);

//! CORDIC法によるarctan
int32_t c_atanl(int32_t y);
//! 2変数arctan
int32_t c_atanl2(int32_t y, int32_t x);

#endif
