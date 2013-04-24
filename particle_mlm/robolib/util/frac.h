/*!
  \file    frac.h
  \ingroup util_frac
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   分数の計算。
 */

#include "install_option.h"
#ifndef __INSTALL_UTIL_FRAC__
#error util_frac is not installed
#endif

#ifndef __INCLUDED_UTIL_FRAC_H__
#define __INCLUDED_UTIL_FRAC_H__



//! 分数構造体
typedef struct {
	int32_t n;	//!< 分子
	int32_t d;	//!< 分母。46340以下ならば桁溢れしないことが保証される。
} frac_t;

#define FRAC(x,y) ((frac_t){x,y})

//! 分数の加算
/*!
  分母は大きい方に合わせる。
 */
frac_t addf(frac_t a, frac_t b);

//! 分数の減算
/*!
  分母は大きい方に合わせる。
 */
frac_t subf(frac_t a, frac_t b);

//! 分数の乗算
/*!
  分母は大きい方に合わせる。
 */
frac_t mulf(frac_t a, frac_t b);

//! 分数の除算
/*!
  分母は大きい方に合わせる。
 */
frac_t divf(frac_t a, frac_t b);

//! 分数の一括加減
/*!
  \param add 足す分数の個数
  \param sub 引く分数の個数
  \param ... 任意個数のfrac_t型変数。足すものを先に並べ、後ろに引くものを並べる。
 */
frac_t sumf(int add, int sub, ...);

//! 分数の一括乗除
/*!
  \param mul 掛ける分数の個数
  \param div 割る分数の個数
  \param ... 任意個数のfrac_t型変数。掛けるものを先に並べ、後ろに割るものを並べる。
  \warning 簡易実装のため、桁あふれへの考慮はまったくない。
 */
frac_t prodf(int mul, int div, ...);

//! 分数の分母が指定値になるよう通分
frac_t redf(frac_t a, int32_t d);

//! フォーマットに従い分数計算を行う。
/*!
  フォーマット一覧
  f frac型変数
  d int型変数
  l long型変数
  D int型変数で表現された分子で、分母はUNIT_MMとしたもの
  L long型変数で表現された分子で、分母はUNIT_MMとしたもの
  + - * / ( ) 演算子

  例: calcff("d+d*(f-d)",3,5,FRAC(20,3),5); // 計算内容: 3+5*(FRAC(20,3)-5)、結果: FRAC(34,3)
 */
frac_t calcff(const char *format, ...);

#endif
