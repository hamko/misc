/*!
  \file    i_math.c
  \ingroup i_math
  \author  Kei Kakitani
  \author  Sota <jine0125@gmail.com>
  \date    2010/07/08

  \brief   マイコン用数学関数
  \todo    STRICT_MODEをもっとふつくしく実装
 */

#include "install_option.h"
#ifdef __INSTALL_I_MATH__

#include "table.h"

#include "i_math.h"
#ifdef MICON
#include <libkairo.h>
#else
#include "stdint.h"
#endif

//! 整数値計算によるsin
int32_t i_sin(int32_t input){
#ifdef STRICT_MODE
    input = arrange_angle(input);
    if( input < -pi/2 ){
        return -sin_table[input+pi];
    }
    else if( input < 0 ){
        return -sin_table[-input];
    }
    else if( input < pi/2 ){
        return sin_table[input];
    }
    else {
        return sin_table[pi-input];
    }
#else
    int32_t retval;
    int16_t index;
    int16_t mod10;
    uint8_t plus;

    plus = 1;

    /* もし、0 ～ 1.57の間でないとき */
    if (input < 0){
        plus = 0;
        input = -input;
    }
    if (input > pi*2){
        input = input % (pi*2);
    }
    if (input > pi){
      input = pi*2 - input;
      plus = 1 - plus;
    }
    if (input > pi/2){
        input = pi - input;
    }

    /* テーブルの値を線形補間(テーブルの精度はSTRICT_MODEの1/10) */
    index = (int16_t)(input / 10);
    mod10 = (int16_t)(input % 10);

    retval = ( (int32_t)sin_table[index] * (10 - mod10)
               + (int32_t)sin_table[index+1] * mod10 
               + 5 ) / 10; // +5は四捨五入用
    if (plus == 0){
        retval = -retval;
    }
    return retval;
#endif
}

//! 整数値計算によるcos
int32_t i_cos(int32_t input){
    return i_sin(input+pi/2);
}

//! 整数値計算によるtan
int32_t i_tan(int32_t input){
    int32_t i_cos_value, mod_pi;

    i_cos_value = i_cos(input);

    /* もし、限りなく±π/2に近いとき */
    if(i_cos_value == 0){ // sin_tableは通常要素数1572なのでinputの1/10000の位を反映できない
        input = input*100 + 1570796; // 1570796 = pi/2(1/1000000まで)
        mod_pi = input % pi - 1570796; // x, y > 0の時、-x / y は -(x / y) mod -(x % y)。ただし本来処理系依存
        if(input < 0){
            mod_pi = mod_pi + 314159;
        }
        if(mod_pi > 0){
            return 2000000000L; // arctan(2000000000) = 1.57079
        }else{
            return -2000000000L;
        }
    }

    if(input>=0){
        return (i_sin(input)*10+5) * 1000 /i_cos(input); // +5は除算誤差補正
    }else{
        return (i_sin(input)*10-5) * 1000 /i_cos(input);
    }
}

//! 整数値計算によるarcsin
int32_t i_arcsin(int32_t input){
#ifdef STRICT_MODE
    if (input > 10000 || input < -10000) /* 門前払い */
        return 0;
    else if( input<0 )
        return -arcsin_table[-input];
    else
        return arcsin_table[input];
#else
    int32_t retval;
    int16_t index;
    int16_t mod10;
    uint8_t plus;

    plus = 1;

    /* ありえないので門前払い */
    if (input > 10000 || input < -10000){
        return 0;
    }

    /* もし、値が負だったら */
    if (input < 0){
        plus = 0;
        input = -input;
    }

    /* テーブルの値を線形補間 */
    index = (int16_t)(input / 10);
    mod10 = (int16_t)(input % 10);

    retval = ( (int32_t)arcsin_table[index] * (10 - mod10)
               + (int32_t)arcsin_table[index+1] * mod10 
               + 5 ) / 10;

    if (plus == 0){
        retval = -retval;
    }

    return retval;
#endif
}

//! 整数値計算によるarccos
int32_t i_arccos(int32_t input){
    return pi/2 + i_arcsin(-input);
}

//! 整数値計算によるarctan
int32_t i_arctan(int32_t input){
    uint16_t r;
    int16_t theta;

    if(input > 32767){ // int16tの最大値 = 32767
        input = 32767;
    }else if(input < -32767){
        input = -32767;
    }
    polar(10000, (int16_t)input, &r, &theta);

    return theta;
}

//! 整数値計算によるarctan
int32_t i_arctan64(int64_t input){
    uint32_t r;
    int32_t theta;

    if(input > 2147483647){ // int16tの最大値 = 32767
        input = 2147483647;
    }else if(input < -2147483647){
        input = -2147483647;
    }
    polar64(10000, (int32_t)input, &r, &theta);

    return theta;
}

//! 角度を-pi ～ piの中に収める
int32_t arrange_angle(int32_t input){
    while(input>pi){
        input -= 2*pi;
    }
    while(input<-pi){
        input += 2*pi;
    }
    return input;
}

//! 整数の平方根を求める(32bit->16bit)
int32_t i_sqrt(uint32_t input){
    uint16_t retval, i;

    retval = 0;

    for(i=32768; i!=0; i=i>>1){
        if ( (uint32_t)(retval+i)*(retval+i) <= input ){
            retval = retval + i; // 最上位から各ビットに対して逐次比較
        }
    }

    return (int32_t)retval;
}

//! 整数の平方根を求める(64bit->32bit)
int64_t i_sqrt64(uint64_t input){
    uint32_t retval, i;

    retval = 0;

    i = 1;
    i <<= (32 - 1);
    for(; i!=0; i=i>>1){
        if ( (uint64_t)(retval+i)*(retval+i) <= input ){
            retval = retval + i; // 最上位から各ビットに対して逐次比較
        }
    }

    return (int64_t)retval;
}


//! 直交座標から極座標に変換する
void polar(int16_t x, int16_t y, uint16_t *r, int16_t *theta){
    uint8_t x_plus, y_plus;

    // 符号情報を保存してひとまず正の領域で扱う
    x_plus = 1; y_plus = 1;
    if (x < 0){
        x_plus = 0;
        x = -x;
    }
    if (y < 0){
        y_plus = 0;
        y = -y;
    }

    *r = (uint16_t)( i_sqrt((int32_t)x*(int32_t)x+(int32_t)y*(int32_t)y) );

    if (x < y){ // 小さい方を取る(桁溢れ防止)
        x = (int16_t)( (int32_t)x * 10000 / *r );
        *theta = (int16_t)i_arccos(x);
    } else {
        y = (int16_t)( (int32_t)y * 10000 / *r );
        *theta = (int16_t)i_arcsin(y);
    }

    if (x_plus == 0 && y_plus == 0){
        *theta = *theta - pi;
    } else if (x_plus == 0 && y_plus == 1){
        *theta = pi - *theta;
    } else if (x_plus == 1 && y_plus == 0){
        *theta = - *theta;
    }
}

//! 直交座標から極座標に変換する
void polar64(int32_t x, int32_t y, uint32_t *r, int32_t *theta){
    uint8_t x_plus, y_plus;

    // 符号情報を保存してひとまず正の領域で扱う
    x_plus = 1; y_plus = 1;
    if (x < 0){
        x_plus = 0;
        x = -x;
    }
    if (y < 0){
        y_plus = 0;
        y = -y;
    }

    *r = (uint32_t)( i_sqrt64((int64_t)x*(int64_t)x+(int64_t)y*(int64_t)y) );

    if (x < y){ // 小さい方を取る(桁溢れ防止)
        x = (int32_t)( (int64_t)x * 10000 / *r );
        *theta = (int32_t)i_arccos(x);
    } else {
        y = (int32_t)( (int64_t)y * 10000 / *r );
        *theta = (int32_t)i_arcsin(y);
    }

    if (x_plus == 0 && y_plus == 0){
        *theta = *theta - pi;
    } else if (x_plus == 0 && y_plus == 1){
        *theta = pi - *theta;
    } else if (x_plus == 1 && y_plus == 0){
        *theta = - *theta;
    }
}


//! x*y/zの形の計算を行う
/*!
  最も計算が簡易で高速。桁溢れしない十分条件はz*yが桁溢れしないこと。
 */
int32_t multdivl(int32_t x, int32_t y, int32_t z)
{
    // コンパイラによっては、負数の余りを正に取ることがある。その場合は(x%z)を(x-((x/z)*z))に書き換える。
    return ((x/z) * y) + ((x%z) * y / z);
}

//! x*y/zの形の計算を行う
/*!
  桁溢れしない十分条件はz*zが桁溢れしないこと。
 */
int32_t multdiv(int32_t x, int32_t y, int32_t z)
{
   int32_t x2 = x%z;
   return ((x/z) * y) + ((y/z) * x2) + (x2 * (y%z) / z);
}

//! x*y/zの形の計算を行う
/*!
  前提として(x/z) * yは桁溢れしないものとする
 */
int32_t multdivs(int32_t x, int32_t y, int32_t z)
{
    const int32_t max_num = ((int32_t) 0x7fffffff);	// 桁あふれするかの監視用。絶対値が一番大きい数-1
    int32_t x_p = x / z, x_q = x % z;	// xをzで割った時の商及び余り。

    // 桁あふれするかの判定。
    if (x_q == 0 || y == 0){
        return x_p * y;
    }
    else if (max_num / x_q / y != 0){
        return ((x_p * y) + (x_q * y / z));
    }

    // やっぱりするようなので今度はyとzでmultiply_divideする
    int32_t y_p = y / z, y_q = y % z;	// yをzで割った時の商及び余り。
    int32_t sum = (x_p * y) + (x_q * y_p);	// これ以後x_p,y_pは不要になる。
    // 桁あふれするかの判定。
    if (y_q == 0){
        return sum;
    }
    else if (max_num / x_q / y_q != 0){
        // x_q * y / zに対してさらにmultiply_divideをかける
        return (sum + (x_q * y_q / z));
    }

    // x_qとy_qはかけると桁溢れするので分ける。
    // 分子分母に(max_num / z) / x_qをかけることで
    // 分子を桁溢れしないことが保証できる範囲で最大にする(y_q < z)。
    return (sum + ((max_num / z) * y_q) / (max_num / x_q));
}

int64_t multiply_divide64(int64_t x, int64_t y, int64_t z)
{
    const int64_t max_num = ((int64_t) 0x7fffffffffffffffll);	// 桁あふれするかの監視用。絶対値が一番大きい数-1
    int64_t x_p = x / z, x_q = x % z;	// xをzで割った時の商及び余り。

    // 桁あふれするかの判定。
    if (x_q == 0 || y == 0){
        return x_p * y;
    }
    else if (max_num / x_q / y != 0){
        return ((x_p * y) + (x_q * y / z));
    }

    // やっぱりするようなので今度はyとzでmultiply_divideする
    int64_t y_p = y / z, y_q = y % z;	// yをzで割った時の商及び余り。
    int64_t sum = (x_p * y) + (x_q * y_p);	// これ以後x_p,y_pは不要になる。
    // 桁あふれするかの判定。
    if (y_q == 0){
        return sum;
    }
    else if (max_num / x_q / y_q != 0){
        // x_q * y / zに対してさらにmultiply_divideをかける
        return (sum + (x_q * y_q / z));
    }

    // x_qとy_qはかけると桁溢れするので分ける。
    // 分子分母に(max_num / z) / x_qをかけることで
    // 分子を桁溢れしないことが保証できる範囲で最大にする(y_q < z)。
    return (sum + ((max_num / z) * y_q) / (max_num / x_q));
}


//! 四捨五入
int32_t i_round(int32_t value, int32_t radix) {
    if(value < 0){
        return ((value - radix / 2)/radix)*radix;
    }
    else {
        return ((value + radix / 2)/radix)*radix;
    }
}

//! numberの符号のみを取り出す
int8_t sgn(int32_t number){
    if(number > 0){
        return 1;
    }else if(number < 0){
        return -1;
    }else{
        return 0;
    }
}

//! 累乗を計算
int32_t i_power(int32_t x, int16_t n, int32_t unit){
    int32_t n_power = 1*unit;
    int16_t i = 0;
    if(n<0){
        return 0;
    }
    for(; i<n; i++){
        n_power = multiply_divide(x,n_power,unit);
    }
    return n_power;
}


#endif
