/*!
  \file    frac.h
  \ingroup util_frac
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   分数の計算。多分煩雑なので使わない。
 */

#include "install_option.h"
#ifndef __INSTALL_UTIL_FRAC__
#error util_frac is not installed
#endif

#ifndef __INCLUDED_UTIL_FRAC_H__
#define __INCLUDED_UTIL_FRAC_H__

#include "r_stdint.h"

//! 分数構造体
typedef struct {
	int32_t n;	//!< 分子
	int32_t d;	//!< 分母
} frac_t;

#define FRAC(x,y) ((frac_t){x,y})

//! 分数の加算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_add(frac_t a, frac_t b);

//! 分数の減算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_sub(frac_t a, frac_t b);

//! 分数の乗算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_mul(frac_t a, frac_t b);

//! 分数の除算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_div(frac_t a, frac_t b);

//! 分数の一括加減
/*!
  \param add 足す分数の個数
  \param sub 引く分数の個数
  \param ... 任意個数のfrac_t型変数。足すものを先に並べ、後ろに引くものを並べる。
 */
frac_t fr_summation(int add, int sub, ...);

//! 分数の一括乗除
/*!
  \param mul 掛ける分数の個数
  \param div 割る分数の個数
  \param ... 任意個数のfrac_t型変数。掛けるものを先に並べ、後ろに割るものを並べる。
  \warning 簡易実装のため、桁あふれへの考慮はまったくない。
 */
frac_t fr_product(int mul, int div, ...);

//! aの分母をbにあわせる
frac_t fr_reduction(frac_t a, frac_t b);

#endif
