/*!
  \file    cordic.c
  \ingroup cordic
  \author  Sota <jine0125@gmail.com>
  \date    2010/09/24

  \brief   CORDIC法による初等関数計算
 */


#include "install_option.h"
#ifdef __INSTALL_CORDIC__

#include "cordic.h"
#include "external.h"

#define CONV(x) ((x)>>(10-SHIFT))   //!< 設定精度に合わせて精度を落とすマクロ

//! 三角関数演算用の係数
#if LOOP == 1
#define TRI_AMP CONV(7240773)
#elif LOOP == 2
#define TRI_AMP CONV(6476345)
#elif LOOP == 3
#define TRI_AMP CONV(6282977)
#elif LOOP == 4
#define TRI_AMP CONV(6234459)
#elif LOOP == 5
#define TRI_AMP CONV(6222318)
#elif LOOP == 6
#define TRI_AMP CONV(6219282)
#elif LOOP == 7
#define TRI_AMP CONV(6218523)
#elif LOOP == 8
#define TRI_AMP CONV(6218333)
#elif LOOP == 9
#define TRI_AMP CONV(6218286)
#elif LOOP == 10
#define TRI_AMP CONV(6218274)
#elif LOOP == 11
#define TRI_AMP CONV(6218271)
#elif LOOP == 12
#define TRI_AMP CONV(6218270)
#elif LOOP == 13
#define TRI_AMP CONV(6218270)
#elif LOOP == 14
#define TRI_AMP CONV(6218270)
#elif LOOP == 15
#define TRI_AMP CONV(6218270)
#endif

//! 双曲線関数演算用の係数
#if LOOP == 1
#define HYP_AMP CONV(13653333)
#elif LOOP == 2
#define HYP_AMP CONV(14563555)
#elif LOOP == 3
#define HYP_AMP CONV(14794723)
#elif LOOP == 4
#define HYP_AMP CONV(14852742)
#elif LOOP == 5
#define HYP_AMP CONV(14867260)
#elif LOOP == 6
#define HYP_AMP CONV(14870891)
#elif LOOP == 7
#define HYP_AMP CONV(14871799)
#elif LOOP == 8
#define HYP_AMP CONV(14872026)
#elif LOOP == 9
#define HYP_AMP CONV(14872082)
#elif LOOP == 10
#define HYP_AMP CONV(14872097)
#elif LOOP == 11
#define HYP_AMP CONV(14872100)
#elif LOOP == 12
#define HYP_AMP CONV(14872101)
#elif LOOP == 13
#define HYP_AMP CONV(14872101)
#elif LOOP == 14
#define HYP_AMP CONV(14872101)
#elif LOOP == 15
#define HYP_AMP CONV(14872101)
#endif
#define C_PI CONV(32169909)    //!< Pi(31416<<SHIFT)

#define mult_div(x,y,z) ((x)/(z)*(y) + (y)/(z)*((x)%(z)) + ((x)%(z))*((y)%(z))/(z))    //!< 簡易multiply_divide

//! 三角関数用arctanテーブル
const int32_t arc[15]={
    CONV(8042477), CONV(4747752), CONV(2508582), CONV(1273395), CONV(639169),
    CONV(319896),  CONV(159987),  CONV(79998),   CONV(40000),   CONV(20000),
    CONV(10000),   CONV(5000),    CONV(2500),    CONV(1250),    CONV(625)
};

//! 双曲線関数用arctanhテーブル
const int32_t arch[15]={
    CONV(5624895), CONV(2615427), CONV(1286730), CONV(640835), CONV(320104),
    CONV(160013),  CONV(80002),   CONV(40000),   CONV(20000),  CONV(10000),
    CONV(5000),    CONV(2500),    CONV(1250),    CONV(625),    CONV(313)
};

//! 三角関数でのrotation mode
static void c_tri_rot(int32_t arg, int32_t* cos, int32_t* sin){
    int32_t x, y, x_new, i;

    x = TRI_AMP;
    y = 0;

    while (arg > 31416){
        arg -= 62832;
    }
    while (arg < -31416){
        arg += 62832;
    }

    if (arg > 15708){
        x = -x;
        arg -= 31416;
    }
    else if (arg < -15708){
        x = -x;
        arg += 31416;
    }

    arg <<= SHIFT;

    for(i=0;i<LOOP;i++){
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

    *cos = x >> SHIFT;
    *sin = y >> SHIFT;

    return;
}


//! 三角関数でのvectoring mode
static void c_tri_vec(int32_t x, int32_t y, int32_t *r, int32_t *atan){
    char n,rev;
    int32_t arg, x_new, i;

    n = 0;
    if (x>0){
        while(x < 10000){
            x <<= 1;
            n++;
        }
    }
    else if (x<0){
        while(x > -10000){
            x <<= 1;
            n++;
        }
    }
    y <<= n;

    rev = 0;
    arg = 0;

    if (x < 0){
        x = -x;
        rev += 1;
        arg = -C_PI;
    }
    if (y < 0){
        y = -y;
        rev += 1;
    }


    for(i=0;i<LOOP;i++){
        if (y < 0){
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

    if (rev==1){
        arg = -arg;
    }


    *atan = arg>>SHIFT;
    *r = x >> n;

    return;
}


//! 双曲線関数でのrotation mode
static void c_hyp_rot(int32_t arg, int32_t *cosh, int32_t *sinh){
    int32_t x, y, x_new, i;
    arg <<= SHIFT;

    x = HYP_AMP;
    y = 0;

    for(i=0;i<(LOOP<<1);i++){
        if (arg > 0){
            x_new = x + (y >> ((i>>1)+1));
            y += x >> ((i>>1)+1);
            x = x_new;
            arg -= arch[(i>>1)];
        }
        else {
            x_new = x - (y >> ((i>>1)+1));
            y -= x >> ((i>>1)+1);
            x = x_new;
            arg += arch[(i>>1)];
        }
    }

    *sinh = y >> SHIFT;
    *cosh = x >> SHIFT;

    return;
}


//! 双曲線関数でのvectoring mode
static void c_hyp_vec(int32_t x, int32_t y, int32_t *r, int32_t *atanh){
    char n;
    int32_t arg, x_new, i;

    n = 0;
    if (x!=0){
        while((x < 10000 && x > 0) || (x > -10000 && x < 0)){
            x <<= 1;
            n++;
        }
    }
    y <<= n;


    arg = 0;

    if (x > 0){
        x = -x;
        y = -y;
    }

    for(i=0;i<(LOOP<<1);i++){
        if (y < 0){
            x_new = x + (y >> ((i>>1)+1));
            y += x >> ((i>>1)+1);
            x = x_new;
            arg -= arch[(i>>1)];
        }
        else {
            x_new = x - (y >> ((i>>1)+1));
            y -= x >> ((i>>1)+1);
            x = x_new;
            arg += arch[(i>>1)];
        }
    }

    *r = x >> n;
    *atanh = arg >> SHIFT;

    return;
}


int32_t c_sin(int32_t arg){
    int32_t cos;
    int32_t sin;
    c_tri_rot(arg, &cos, &sin);
    return sin;
}

int32_t c_cos(int32_t arg){
    int32_t cos;
    int32_t sin;
    c_tri_rot(arg, &cos, &sin);
    return cos;
}

int32_t c_tan(int32_t arg){
    int32_t cos;
    int32_t sin;
    c_tri_rot(arg, &cos, &sin);
    return mult_div(10000, sin, cos);
}

int32_t c_asin(int32_t z){
    int32_t arg, x, y, x_new, i;
    z <<= SHIFT;

    x = TRI_AMP;
    y = 0;
    arg = 0;

    for(i=0;i<LOOP;i++){
        if (y < z){
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

    return -arg >> SHIFT;    // theta
}

int32_t c_acos(int32_t z){
    return 15707 - c_asin(z);
}

int32_t c_atan(int32_t x, int32_t y){
    int32_t r;
    int32_t atan;
    c_tri_vec(x, y, &r, &atan);
    return atan;
}

int32_t c_sinh(int32_t arg){
    int32_t cosh;
    int32_t sinh;
    c_hyp_rot(arg, &cosh, &sinh);
    return sinh;
}

int32_t c_cosh(int32_t arg){
    int32_t cosh;
    int32_t sinh;
    c_hyp_rot(arg, &cosh, &sinh);
    return cosh;
}

int32_t c_tanh(int32_t arg){
    int32_t cosh;
    int32_t sinh;
    c_hyp_rot(arg, &cosh, &sinh);
    return mult_div(10000, sinh, cosh);
}

int32_t c_atanh(int32_t x, int32_t y){
    int32_t atanh;
    int32_t r;
    c_hyp_vec(x, y, &r, &atanh);
    return atanh;
}

int32_t c_exp(int32_t arg){
    int32_t cosh;
    int32_t sinh;
    c_hyp_rot(arg, &cosh, &sinh);
    return cosh+sinh;
}

int32_t c_log(int32_t arg){
    return 2*c_atanh(arg+10000, arg-10000);
}

int32_t c_sqrt(int32_t x){
    int32_t atanh;
    int32_t r;
    c_hyp_vec(x+2500, x-2500, &r, &atanh);

    return mult_div(r, HYP_AMP>>SHIFT, 10000);
}

void c_polar(int32_t x, int32_t y, int32_t *r, int32_t *theta){
    c_tri_vec(x, y, r, theta);
    *r = mult_div(*r, TRI_AMP>>SHIFT, 10000);
    return;
}
#endif
