#include "install_option.h"
#ifndef __INSTALL_I_MATH_S__
#error i_math_s is not installed
#endif

#ifndef __INCLUDED_I_MATH_S_H__
#define __INCLUDED_I_MATH_S_H__

#include "r_stdint.h"
#include "i_math.h"
#include "map.h"

//i_power
//xのn乗を計算する。unitは最小単位の逆数。
//n>=0のみ対応。
int32_t i_power(int32_t x, int16_t n,int32_t unit);

//i_abs
//絶対値を返す
int32_t i_abs(int32_t number);


// zeroからのposの相対座標を求める
map_t get_relative_pos(p_map_t zero,map_t pos);
//zeroからのposの相対距離を求める
int32_t get_relative_distance(map_t zero,map_t pos);

#endif
