/*!
  \file    frac.c
  \ingroup util_frac
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   分数の計算。多分煩雑なので使わない。
 */

#include "install_option.h"
#ifdef __INSTALL_UTIL_FRAC__

#include <stdarg.h>
#include "frac.h"
#include "r_stdint.h"


//! x*y/zを精度良く計算する
static inline int32_t mult_div(int32_t x, int32_t y, int32_t z)
{
	return (x / z) * y + (y / z) * (x%z) + (x%z)*(y%z)/z;
}

//! 分数の加算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_add(frac_t a, frac_t b)
{
	if (a.d == b.d){
		return FRAC(a.n + b.n, a.d);
	}
	else if (a.d > b.d){
		return FRAC(a.n + mult_div(b.n,b.d,a.d), a.d);
	}
	else {
	    return FRAC(b.n + mult_div(a.n,a.d,b.d), b.d);
	}
}

//! 分数の減算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_sub(frac_t a, frac_t b)
{
	if (a.d == b.d){
		return FRAC(a.n - b.n, a.d);
	}
	else if (a.d > b.d){
		return FRAC(a.n - mult_div(b.n,b.d,a.d), a.d);
	}
	else {
		return FRAC(mult_div(a.n,a.d,b.d) - b.n, b.d);
	}
}

//! 分数の乗算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_mul(frac_t a, frac_t b)
{
	if (a.d >= b.d){
		return FRAC(mult_div(a.n, b.n, b.d), a.d);
	}
	else {
	    return FRAC(mult_div(b.n, a.n, a.d), b.d);
	}
}


//! 分数の除算
/*!
  分母は大きい方に合わせる。
 */
frac_t fr_div(frac_t a, frac_t b)
{
	if (a.d >= b.d){
	    return FRAC(mult_div(a.n, b.d, b.n), a.d);
	}
	else {
		return FRAC(mult_div(mult_div(b.d, b.d, a.d), a.n, b.n), b.d);
	}
}


//! 分数の一括加減
/*!
  \param add 足す分数の個数
  \param sub 引く分数の個数
  \param ... 任意個数のfrac_t型変数。足すものを先に並べ、後ろに引くものを並べる。
 */
frac_t fr_summation(int add, int sub, ...)
{
    int i;
    frac_t result;
    va_list ap;

    va_start(ap, sub);
    result = va_arg(ap, frac_t);

    for (i=0;i<add;i++){
        result = fr_add(result, va_arg(ap, frac_t));
    }
    for (i=0;i<sub;i++){
        result = fr_sub(result, va_arg(ap, frac_t));
    }

    va_end(ap);

    return result;
}


//! 分数の一括乗除
/*!
  \param add 掛ける分数の個数
  \param sub 割る分数の個数
  \param ... 任意個数のfrac_t型変数。掛けるものを先に並べ、後ろに割るものを並べる。
  \warning 簡易実装のため、桁あふれへの考慮はまったくない。
 */
frac_t fr_product(int mul, int div, ...)
{
    int i;
    frac_t result;
    va_list ap;

    va_start(ap, div);
    result = va_arg(ap, frac_t);

    for (i=0;i<mul;i++){
        result = fr_mul(result, va_arg(ap, frac_t));
    }
    for (i=0;i<div;i++){
        result = fr_div(result, va_arg(ap, frac_t));
    }

    va_end(ap);

    return result;
}


//! aの分母をbにあわせる
frac_t fr_reduction(frac_t a, frac_t b)
{
    return FRAC(mult_div(a.n,b.d,a.d), b.d);
}


// tips:

// int32_tの最大値は2147483647。
// *10000の世界では、214748までしか扱えない。

// mult_divで桁あふれしないことが保証されているのはz=46340まで。
// すなわち、.dに許されるのは46340まで。

#endif
