/*!
  \file    i_math.h
  \ingroup i_math
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/12

  \brief   マイコン用数学関数
 */

#include "install_option.h"
#ifndef __INSTALL_I_MATH__
#error i_math is not installed
#endif

#ifndef __INCLUDED_I_MATH_H__
#define __INCLUDED_I_MATH_H__

#include "r_stdint.h"

//! 円周率 [rad]*10000
#undef pi
#define pi (int32_t)31416

//! 数値の絶対値を返す
#define ABS(a) ((a)<0?(-(a)):(a))
//! 2つの数値のうち大きい方を返す
#define MAX(a,b) ((a)>(b)?(a):(b))
//! 2つの数値のうち小さい方を返す
#define MIN(a,b) ((a)<(b)?(a):(b))

//! 整数値計算によるsin
/*!
  \param theta 角度[rad]*10000
 */
int32_t i_sin(int32_t theta);

//! 整数値計算によるcos
/*!
  \param theta 角度[rad]*10000
 */
int32_t i_cos(int32_t theta);


//! 整数値計算によるtan
/*!
  \param theta 角度[rad]*10000
  \return -∞ ～ ∞ (*10000)
 */
int32_t i_tan(int32_t theta);

//! 整数値計算によるarcsin
/*!
  \param val y*10000
  \return -pi/2 ～ pi (*10000)
 */
int32_t i_arcsin(int32_t val);

//! 整数値計算によるarccos
/*!
  \param val x*10000
  \return 0 ～ pi (*10000)
 */
int32_t i_arccos(int32_t val);

//! 整数値計算によるarctan
/*!
  \param val y (tan-1 ～ tan1) *10000
  \return -1rad ～ 1rad (*10000)
 */
int32_t i_arctan(int32_t val);

//! 角度を-pi ～ piの中に収める
/*!
  \param angle 角度[rad]*10000
 */
int32_t arrange_angle(int32_t angle);

//! 整数の平方根を求める(32bit->16bit)
int32_t i_sqrt(uint32_t val);

//! 直交座標から極座標に変換する
/*!
  \param x x座標
  \param y y座標
  \param r 変換後、r座標を代入するポインタ(uint16_t)
  \param theta 変換後、theta座標を代入するポインタ
  代入される値の範囲は-pi ～ pi [rad]*10000
 */
void polar(int16_t x, int16_t y, uint16_t *r, int16_t *theta);

//! x*y/zの形の計算を行う
int32_t multiply_divide(int32_t x, int32_t y, int32_t z);
int64_t multiply_divide64(int64_t x, int64_t y, int64_t z);

//! 四捨五入
/*!
  \param value 丸める値
  \param radix 基数(10進数なら10と指定すれば1の位が四捨五入される)
 */
int32_t i_round(int32_t value, int32_t radix);

//! numberの符号のみを取り出す
/*!
  (int32_t -> int8_t)
 */
int8_t sgn(int32_t number);


#endif
