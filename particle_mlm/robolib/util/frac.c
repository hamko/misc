/*!
  \file    frac.c
  \ingroup util_frac
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   分数の計算。
 */

#include "install_option.h"
#ifdef __INSTALL_UTIL_FRAC__

#include <stdarg.h>

#include "frac.h"
#include "unit_system.h"

//! x*y/zを精度良く計算する
static inline int32_t mult_div(int32_t x, int32_t y, int32_t z)
{
    return (x / z) * y + (y / z) * (x%z) + (x%z)*(y%z)/z;
}

//! 分数の加算
/*!
  分母は大きい方に合わせる。
 */
frac_t addf(frac_t a, frac_t b)
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
frac_t subf(frac_t a, frac_t b)
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
frac_t mulf(frac_t a, frac_t b)
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
frac_t divf(frac_t a, frac_t b)
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
frac_t sumf(int add, int sub, ...)
{
    int i;
    frac_t result;
    va_list ap;

    va_start(ap, sub);
    result = va_arg(ap, frac_t);

    for (i=0;i<add;i++){
        result = addf(result, va_arg(ap, frac_t));
    }
    for (i=0;i<sub;i++){
        result = subf(result, va_arg(ap, frac_t));
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
frac_t prodf(int mul, int div, ...)
{
    int i;
    frac_t result;
    va_list ap;

    va_start(ap, div);
    result = va_arg(ap, frac_t);

    for (i=0;i<mul;i++){
        result = mulf(result, va_arg(ap, frac_t));
    }
    for (i=0;i<div;i++){
        result = divf(result, va_arg(ap, frac_t));
    }

    va_end(ap);

    return result;
}


//! 分数の分母が指定値になるよう通分
frac_t redf(frac_t a, int32_t d)
{
    return FRAC(mult_div(a.n,d,a.d), d);
}



//! 内部処理
/*!
  \param head   ()の先頭であることを示すフラグ
  \param format フォーマット
  \param n      読み取り済文字数へのポインタ
  \param ap     フォーマットの中身
 */
static frac_t calcff_va(char head, const char *format, int *n, va_list ap)
{
    frac_t f = FRAC(0,1);
    while (1){
        switch (format[(*n)++]){
        case '+':
            if (!head){
                (*n)--;
                return f;
            }
            f = addf(f, calcff_va(0, format, n, ap));
            break;
        case '-':
            if (!head){
                (*n)--;
                return f;
            }
            f = subf(f, calcff_va(0, format, n, ap));
            break;
        case '*':
            f = mulf(f, calcff_va(0, format, n, ap));
            break;
        case '/':
            f = divf(f, calcff_va(0, format, n, ap));
            break;

        case 'f':
            f = va_arg(ap, frac_t);
            break;
        case 'd':
            f = FRAC(va_arg(ap, int),1);
            break;
        case 'l':
            f = FRAC(va_arg(ap, long),1);
            break;
        case 'D':
            f = FRAC(va_arg(ap, int), UNIT_MM);
            break;
        case 'L':
            f = FRAC(va_arg(ap, long), UNIT_MM);
            break;

        case '(':
            f = calcff_va(1, format, n, ap);
            break;
        case ')':
        case '\0':
            (*n)--;
            return f;
        }
    }
}

//! フォーマットに従い分数計算を行う。
/*!
  フォーマット一覧
  f frac型変数
  d int型変数
  l long型変数
  D int型変数で表現された分子、分母はUNIT_MMとしたもの
  L long型変数で表現された分子、分母はUNIT_MMとしたもの
  + - * / ( ) 演算子

  例: calcff("d+d*(f-d)",3,5,FRAC(20,3),5); // 計算内容: 3+5*(FRAC(20,3)-5)、結果: FRAC(34,3)
 */
frac_t calcff(const char *format, ...)
{
    frac_t f;
    va_list ap;
    int n = 0;

    va_start(ap, format);
    f = calcff_va(1, format, &n, ap);
    va_end(ap);

    return f;
}



// tips:

// int32_tの最大値は2147483647。
// *10000の世界では、214748までしか扱えない。

// mult_divで桁あふれしないことが保証されているのはz=46340まで。
// すなわち、.dに許されるのは46340まで。


#endif