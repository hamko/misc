/*!
  \file    cordic64.c
  \ingroup cordic
  \author  Sota <jine0125@gmail.com>
  \date    2011/12/21

  \brief   CORDIC法をint64_t型を使うことでより高精度な値を返す。unit_system.hで宣言されたUNIT_MM_LとUNIT_RAD_Lで倍率をする。
 */


#include "install_option.h"
#ifdef __INSTALL_CORDIC64__


//#include <libkairo.h>
#include <stdint.h>
#include "cordic64.h"
#include "unit_system.h"


//! 計算回数(1-25回)
static const int16_t LOOP_64=25;

//! 三角関数演算用の係数
#if LOOP_64 == 1
static const int32_t TRI_AMP=60725294;
#elif LOOP_64 == 2
static const int32_t TRI_AMP_L=63245553;
#elif LOOP == 3
static const int32_t TRI_AMP_L=61357199;
#elif LOOP == 4
static const int32_t TRI_AMP_L=60883391;
#elif LOOP == 5
static const int32_t TRI_AMP_L=60764826;
#elif LOOP == 6
static const int32_t TRI_AMP_L=60735177;
#elif LOOP == 7
static const int32_t TRI_AMP_L=60727764;
#elif LOOP == 8
static const int32_t TRI_AMP_L=60725911;
#elif LOOP == 9
static const int32_t TRI_AMP_L=60725448;
#elif LOOP == 10
static const int32_t TRI_AMP_L=60725332;
#elif LOOP == 11
static const int32_t TRI_AMP_L=60725303;
#elif LOOP == 12
static const int32_t TRI_AMP_L=60725296;
#else
static const int32_t TRI_AMP_L=60725294;
#endif

static const int32_t C_UNIT_MM_L=100000000;

static const uint32_t C_PI_L=(uint32_t)314159265;
static const int32_t C_UNIT_RAD_L=100000000;

//! 三角関数用arctanテーブル
static const int32_t arc[25]={
  78539816,
  46364761,
  24497866,
  12435499,
  6241881,
  3123983,
  1562373,
  781234,
  390623,
  195312,
  97656,
  48828,
  24414,
  12207,
  6104,
  3052,
  1526,
  763,
  381,
  191,
  95,
  48,
  24,
  12,
  6
};

static int32_t _arrange_angle(int32_t arg){
    const int64_t c_pi = (C_PI_L+(C_UNIT_RAD_L/UNIT_RAD_L)/2)/(C_UNIT_RAD_L/UNIT_RAD_L);
    while(arg>c_pi){
        arg -= 2*c_pi;
    }
    while(arg < -c_pi){
        arg += 2*c_pi;
    }
    return arg;
}

//! 三角関数でのrotation mode
static void c_tri_rotl(int32_t arg, int32_t* cos, int32_t* sin){
    int64_t x, y, x_new;
    int32_t i;
    const int32_t c_pi = (C_PI_L+(C_UNIT_RAD_L/UNIT_RAD_L)/2)/(C_UNIT_RAD_L/UNIT_RAD_L);
    arg = _arrange_angle(arg);

    x = TRI_AMP_L;
    y = 0;


    if (arg > c_pi/2){
        x = -x;
        arg -= c_pi;
    }
    else if (arg < -c_pi/2){
        x = -x;
        arg += c_pi;
    }
    arg *= C_UNIT_RAD_L/UNIT_RAD_L;

    for(i=0;i<LOOP_64;i++){
        if (arg > 0){
            x_new = x - (y >> i);
            y += x >> i;
            x = x_new;
            arg -= arc[i];
        }
        else {
            x_new = x + (y >> i);
            y -= x >> i;
            x = x_new;
            arg += arc[i];
        }
    }

    *cos = x/(C_UNIT_MM_L/UNIT_MM_L);
    *sin = y/(C_UNIT_MM_L/UNIT_MM_L);
    return;
}

//! 三角関数でのvectoring mode
static void c_tri_vecl(int32_t x, int32_t y, int32_t *r, int32_t *atan){
    char n,rev;
    int32_t arg, i;
    int64_t _x,_y,x_new;

    n = 0;
    
    rev = 0;
    arg = 0;

    if (x < 0){
        x = -x;
        rev += 1;
        arg = -C_PI_L;
    }
    if (y < 0){
        y = -y;
        rev += 1;
    }
    _x =(int64_t) x << (LOOP_64-1);
    _y =(int64_t) y << (LOOP_64-1);


    for(i=0;i<LOOP_64;i++){
        if (_y < 0){
            x_new = _x - (_y >> i);
            _y += _x >> i;
            _x = x_new;
            arg -= arc[i];
        }
        else {
            x_new = _x + (_y >> i);
            _y -= _x >> i;
            _x = x_new;
            arg += arc[i];
        }
    }

    if (rev==1){
        arg = -arg;
    }
    _x = (_x + (1 << (LOOP_64-2))) >> (LOOP_64-1);

    *atan = (arg+(C_UNIT_RAD_L/UNIT_RAD_L)/2)/(C_UNIT_RAD_L/UNIT_RAD_L);
    *r = (int32_t)(_x+(C_UNIT_MM_L/UNIT_MM_L)/2)/(C_UNIT_MM_L/UNIT_MM_L);

    return;
}

int32_t c_sinl(int32_t arg){
    int32_t cos;
    int32_t sin;
    c_tri_rotl(arg, &cos, &sin);
    return sin;
}

int32_t c_cosl(int32_t arg){
    int32_t cos;
    int32_t sin;
    c_tri_rotl(arg, &cos, &sin);
    return cos;
}

int32_t c_atanl2(int32_t y, int32_t x){
    int32_t r;
    int32_t atan;
    c_tri_vecl(x, y, &r, &atan);
    return atan;
}

int32_t c_atanl(int32_t y){
    int32_t r;
    int32_t atan;
    c_tri_vecl(UNIT_MM_L, y, &r, &atan);
    return atan;
}

#endif
