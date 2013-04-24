/*!
  \file    cordic.h
  \ingroup cordic
  \author  Sota <jine0125@gmail.com>
  \date    2010/09/24

  \brief   CORDIC法による初等関数計算
 */

#include "install_option.h"
#ifndef __INSTALL_CORDIC__
#error cordic is not installed
#endif

#ifndef __INCLUDED_CORDIC_H__
#define __INCLUDED_CORDIC_H__

#include "external.h"

//! CORDIC法によるsin
int32_t c_sin(int32_t arg);
//! CORDIC法によるcos
int32_t c_cos(int32_t arg);
//! CORDIC法によるtan
int32_t c_tan(int32_t arg);

//! CORDIC法によるarcsin
int32_t c_asin(int32_t z);
//! CORDIC法によるarccos
int32_t c_acos(int32_t z);
//! CORDIC法によるarctan
int32_t c_atan(int32_t x, int32_t y);

//! 極座標変換
void c_polar(int32_t x, int32_t y, int32_t *r, int32_t *theta);

//! CORDIC法によるhyperboricsin
int32_t c_sinh(int32_t arg);
//! CORDIC法によるhyperboriccos
int32_t c_cosh(int32_t arg);
//! CORDIC法によるhyperborictan
int32_t c_tanh(int32_t arg);

//! CORDIC法によるarchyperborictan
int32_t c_atanh(int32_t x, int32_t y);

//! CORDIC法によるexp
int32_t c_exp(int32_t arg);
//! CORDIC法によるlog
int32_t c_log(int32_t arg);
//! CORDIC法によるsqrt
int32_t c_sqrt(int32_t x);
#endif
